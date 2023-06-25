#!/usr/bin/env python
# coding: utf-8

from math import factorial as f

def combination(k, n):
    return float(f(n)) / (f(k) * f(n-k))

def arrangement(k, n):
    return float(f(n)) / f(n-k)

C = combination  # alias
A = arrangement  # alias

def assert_almost_equal(left, right):
    diff = abs(left - right)
    if diff >= 1e-6:
        print "ERROR !"
        print "Left  = ", left
        print "Right = ", right
        print "Diff  = ", diff
        raise ValueError(diff)

def display_percent(float_value):
    percentage_str = "{:4.1f} %".format(float_value*100)
    one_over_N = 1.0 / float_value
    one_over_N_str = "{:4.1f}".format(one_over_N)
    return "{}   --   1 partie sur {}".format(percentage_str, one_over_N_str)
