#!/usr/bin/env python
# -*- coding: utf-8 -*-

class Luke(object):
    def iamyourfather(self):
        with open("/tmp/poc.log", "a") as f:
            f.write("Nooooooooo !\n")
