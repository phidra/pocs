#!/usr/bin/env python3
# coding: utf-8

# inspired by Raymond Hettinger's talk "Thinking about concurrency"
# https://www.youtube.com/watch?v=Bv25Dwe84g0

# in this step, work is shared amongs threads, and work is synchronized
# using atomic message queue. This implementation differs of Raymond's
# in that the workers actually update the counter themselves.
# But in fine, the work is done sequentially : there is never more
# that one thread that is working at the same time.

import threading
import queue
import random
import time

NB_ITERATIONS = 10
counter = 0
ADD_RANDOM_DELAY = True  # even with delay, all is ok

counter_queue = queue.Queue()
printer_queue = queue.Queue()


def random_delay():
    """ Add delay to mess with thread execution order"""
    if not ADD_RANDOM_DELAY:
        return
    nb_ms = random.randint(0, 100)
    time.sleep(nb_ms / 1000)


def worker():
    random_delay()
    counter_before = counter_queue.get()
    random_delay()
    counter_after = counter_before + 1
    random_delay()
    counter_queue.task_done()
    random_delay()
    counter_queue.put(counter_after)
    random_delay()
    printer_queue.put("working on : %i" % counter_after)
    random_delay()
    global counter
    counter = counter_after


def printer():
    """ Daemon thread to print things, read from a queue"""
    while True:
        random_delay()
        msg = printer_queue.get()
        random_delay()
        print(msg)
        random_delay()
        printer_queue.task_done()
        random_delay()


if __name__ == "__main__":
    print("Startup")

    # start printer thread :
    printer_thread = threading.Thread(target=printer, daemon=True)
    printer_thread.start()

    # trig initial work :
    counter_queue.put(counter)

    # start working threads :
    working_threads = []
    for _ in range(NB_ITERATIONS):
        t = threading.Thread(target=worker)
        t.start()
        working_threads.append(t)

    # wait for working threads to complete :
    for t in working_threads:
        t.join()

    # wait for all work to be done (possibly : useless if we join threads) :
    counter_queue.task_done()
    counter_queue.join()

    # wait for all messages to be printed :
    printer_queue.join()

    print("Ending, final value of counter is %i" % counter)
