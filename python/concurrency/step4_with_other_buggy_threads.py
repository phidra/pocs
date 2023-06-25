#!/usr/bin/env python3
# coding: utf-8

# inspired by Raymond Hettinger's talk "Thinking about concurrency"
# https://www.youtube.com/watch?v=Bv25Dwe84g0

# in this step, work is shared amongs threads, but in a buggy way :
# execution order is messed up by random delay
# this is an alternative implementation of step3 without ThreadPoolExecutor

import threading
import random
import time

NB_ITERATIONS = 10
counter = 0
ADD_RANDOM_DELAY = True  # without delay, all (falsely) SEEMS to be ok


def random_delay():
    """ Add delay to mess with thread execution order"""
    if not ADD_RANDOM_DELAY:
        return
    nb_ms = random.randint(0, 100)
    time.sleep(nb_ms / 1000)


def worker():
    random_delay()
    global counter
    random_delay()
    counter += 1
    random_delay()
    print("working on : %i" % counter)
    random_delay()


if __name__ == "__main__":
    print("Startup")
    threads = []
    for _ in range(NB_ITERATIONS):
        t = threading.Thread(target=worker)
        t.start()
        threads.append(t)
    for t in threads:
        t.join()
    print("Ending, final value of counter is %i" % counter)
