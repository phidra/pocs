#!/usr/bin/env python3
# coding: utf-8

# inspired by Raymond Hettinger's talk "Thinking about concurrency"
# https://www.youtube.com/watch?v=Bv25Dwe84g0

# in this step, work is shared amongs threads, and work is synchronized
# using atomic message queue.
# A daemon thread is responsible to print, and another daemon thread
# is responsible to increment counter.
# According to Raymond, it is the proper way to multithread this simple problem

import threading
import queue
import random
import time

NB_ITERATIONS = 10
counter = 0
ADD_RANDOM_DELAY = True  # even with delay, all is ok :-)

printer_queue = queue.Queue()
incrementer_queue = queue.Queue()


def random_delay():
    """ Add delay to mess with thread execution order"""
    if not ADD_RANDOM_DELAY:
        return
    nb_ms = random.randint(0, 100)
    time.sleep(nb_ms / 1000)


def worker():
    random_delay()
    incrementer_queue.put(1)


def printer():
    """ Daemon thread to print things, only one allowed to use 'print' """
    while True:
        random_delay()
        msg = printer_queue.get()
        random_delay()
        print(msg)
        random_delay()
        printer_queue.task_done()
        random_delay()


def incrementer():
    """Daemon thread to increment counter, only one allowed to write counter"""
    while True:
        random_delay()
        value_to_add = incrementer_queue.get()
        random_delay()
        global counter
        counter += value_to_add
        random_delay()
        printer_queue.put("working on : %i" % counter)
        random_delay()
        incrementer_queue.task_done()
        random_delay()


if __name__ == "__main__":

    # start daemon threads :
    printer_thread = threading.Thread(target=printer, daemon=True)
    printer_thread.start()
    printer_queue.put("Startup")
    incrementer_thread = threading.Thread(target=incrementer, daemon=True)
    incrementer_thread.start()

    # start working threads :
    working_threads = []
    for _ in range(NB_ITERATIONS):
        t = threading.Thread(target=worker)
        t.start()
        working_threads.append(t)

    # wait for working threads to complete :
    for t in working_threads:
        t.join()

    # wait for all messages to be printed :
    incrementer_queue.join()
    printer_queue.put("Ending, final value of counter is %i" % counter)
    printer_queue.join()
