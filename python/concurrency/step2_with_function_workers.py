#!/usr/bin/env python3
# coding: utf-8

# inspired by Raymond Hettinger's talk "Thinking about concurrency"
# https://www.youtube.com/watch?v=Bv25Dwe84g0

# in this step, work is spread amongst worker functions but still done linearly

NB_ITERATIONS = 10
counter = 0


def function_worker():
    global counter
    counter += 1
    print("working on : %i" % counter)


if __name__ == "__main__":
    print("Startup")
    for i in range(NB_ITERATIONS):
        function_worker()
    print("Ending, final value of counter is %i" % counter)
