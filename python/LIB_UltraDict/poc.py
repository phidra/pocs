#!/usr/bin/env python3

from multiprocessing import Pool, Manager
import os
import time
from itertools import cycle
import sys
import statistics

import psutil
from UltraDict import UltraDict


def size_in_mio(size_in_bytes: float) -> str:
    return f"{size_in_bytes / (1024 * 1024):.1f} Mio"


def print_rss_of_empty_subprocess():
    process = psutil.Process(os.getpid())
    rss_in_bytes = process.memory_info().rss
    print(f"An empty process actually uses {size_in_mio(rss_in_bytes)}")
    print("")


def use_all_keys_and_values(dictionary):
    # this ensures that we read all keys and values of dictionary
    keys_result = [None, None, None, None]
    c = cycle(range(4))
    total_values = 0
    for key, value in dictionary.items():
        keys_result[next(c)] = key[0]
        total_values += value

    return keys_result, total_values


def process_dict(stop_event, rss_of_subprocesses, data, process_id):
    """
    This function will be called in subprocessed
    """
    keys_result, total_values = use_all_keys_and_values(data)

    # print subprocess RSS :
    process = psutil.Process(os.getpid())
    rss_in_bytes = process.memory_info().rss
    print(f"The process #{process_id} uses {size_in_mio(rss_in_bytes)}")

    # send subprocess RSS to parent :
    rss_of_subprocesses.append(rss_in_bytes)

    # wait for parent to "kill" this subprocess :
    stop_event.wait()

    return keys_result, total_values, rss_in_bytes


def process_ultra_dict(stop_event, rss_of_subprocesses, ultra_dict_name, process_id):
    """
    This function will be called in subprocessed
    """
    ultra = UltraDict(name=ultra_dict_name)
    process_dict(stop_event, rss_of_subprocesses, ultra.data, process_id)


def display_dict_size(data):
    # dict size is the sum of 1. dict itself and 2. its data :
    dict_size = sys.getsizeof(data)
    items_size = 0
    for key, value in data.items():
        items_size += sys.getsizeof(key) + sys.getsizeof(value)
    total_size = dict_size + items_size
    print(f"TOTAL DICT SIZE = {size_in_mio(total_size)}")
    print(f"({size_in_mio(dict_size)} for the dict itself, and {size_in_mio(items_size)} for its items)")
    print("")


def main():
    print("===== STEP 0 = creating data :")
    nb_items = 200000
    data = {str(x): x * x for x in range(nb_items)}
    display_dict_size(data)

    print("===== STEP 1 = parallel processing data, with regular dict :")
    num_subprocesses = 20
    launch_subprocesses_that_share_a_dict(num_subprocesses, process_dict, subprocess_data=data)

    print("===== STEP 2 = parallel processing data, with UltraDict :")
    print("building UltraDict from data (this might take a while)")
    start = time.time()
    ultra = UltraDict(data)
    elapsed = time.time() - start
    print(f"finished building ultradict in {elapsed:.1f} seconds")
    launch_subprocesses_that_share_a_dict(num_subprocesses, process_ultra_dict, subprocess_data=ultra.name)


def launch_subprocesses_that_share_a_dict(num_subprocesses, subprocess_function, subprocess_data):
    """
    This is where the magic happens : we dispatch work amongst subprocesses in a pool
    We expect subprocesses to report their RSS.
    When all subprocesses dit that, we send them an event to stop them.
    """

    # parent process RSS :
    process = psutil.Process(os.getpid())
    parent_rss_in_bytes = process.memory_info().rss
    print(f"PARENT PROCESS RSS : {size_in_mio(parent_rss_in_bytes)}")

    manager = Manager()
    rss_of_subprocesses = manager.list()
    stop_event = manager.Event()
    with Pool(num_subprocesses) as pool:

        # measuring RSS of an empty subprocess :
        pool.apply(print_rss_of_empty_subprocess)

        results = []
        for i in range(num_subprocesses):
            res = pool.apply_async(subprocess_function, (stop_event, rss_of_subprocesses, subprocess_data, i))
            results.append(res)

        pool.close()

        # ici, on devrait plutôt se faire signaler (mais pour la POC, un sleep est suffisant)
        print("Waiting for subprocesses to complete...")
        while len(rss_of_subprocesses) < num_subprocesses:
            time.sleep(0.2)

        # signal subprocesses to exit
        print("Ok, all subprocesses reported their RSS, we can exit now")
        stop_event.set()

        print("")
        print("Waiting for subprocesses to complete")
        pool.join()
        print("Done")

        print("rss_of_subprocesses = ", rss_of_subprocesses)
        average_rss_bytes = statistics.fmean(rss_of_subprocesses)
        print(f"AVG subprocess RSS = {size_in_mio(average_rss_bytes)}")
        total_rss = parent_rss_in_bytes + sum(rss_of_subprocesses)
        print(f"TOTAL = {size_in_mio(total_rss)}")
        print("")


if __name__ == "__main__":
    description = """
    POSITION DU PROBLÈME :
        on veut partager du travail CPU-bound entre plusieurs workers
        le travail étant CPU-bound, on utilise plusieurs process plutôt que plusieurs threads
        les workers ont besoin d'accéder en lecture à une grosse structure de donnée (e.g. le cache d'une DB)
        l'objectif est que tous les process utilisent une même structure pour éviter de la dupliquer en RAM
        (l'idée est de réussir à utiliser un dict plutôt qu'une in-memory database comme redis)

    CE QUE FAIT LA POC :
        une fonction accepte un dict, et travaille dessus (en touchant chaque clé+valeur)
        on utilise plein de workers en leur passant le dict, et on mesure l'occupation mémoire des sous-process
        version 1 = on passe le dict python brut aux sous-process
        version 2 = on passe la shared-memory name d'un UltraDict aux sous-process
        NOTE : on mesure aussi la conso mémoire d'un subprocess vide pour bien voir ce qui est dû aux données partagées
        CAVEAT : c'est le RSS qu'on mesure (et il y a sans doute des pages partagées par tous les process)


    CE QUE MONTRE LA POC :
        si on passe un même dict à tous les process, celui-ci est dupliqué en RAM, avec une copie pour chaque process
        la tentative d'utiliser un UltraDict ne tient pas ses promesses, chaque process a tout de même sa copie locale
        (c'est logique a posteriori : on ne peut pas partager des objets python entre des interpréteurs différents)

    """
    print(description)
    main()
