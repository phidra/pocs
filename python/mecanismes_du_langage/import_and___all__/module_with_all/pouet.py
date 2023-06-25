#!/usr/bin/env python
# -*- coding: utf-8 -*-

class Pouet(object):
    def youpi(self):
        with open("/tmp/poc.log", "a") as f:
            f.write("Youpi !\n")
