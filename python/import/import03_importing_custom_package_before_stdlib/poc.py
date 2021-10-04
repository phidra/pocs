#!/usr/bin/env python3

import pprint

print(
    """
    This POC shows that when importing a module 'spam', the modules/packages in the same folder as the main one
    are searched BEFORE the stdlib.

    Indeed : this POC calls pprint.pprint, and it's the one from our CUSTOM pprint package that is used.
"""
)

pprint.pprint("coucou")
