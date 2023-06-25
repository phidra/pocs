#!/usr/bin/env python
# coding: utf-8

import random
import time

from bottle import route, run

number = int(__file__[-4]) # file name is serverX.py
PORT = 8080 + number
print "LISTENING : ", PORT

@route('/')
def index():
    time.sleep(3)
    return str(PORT)

run(host='localhost', port=PORT)
