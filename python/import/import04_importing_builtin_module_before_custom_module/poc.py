#!/usr/bin/env python3

import math

print(
    """
    This POC shows that when importing a module, the builtin modules are used BEFORE custom ones.

    Indeed : this POC calls math.sqrt, and even if we HAVE a custom sqrt, it's the builtin one that is used.
"""
)

a = math.sqrt(2)
print(f"sqrt(2) = {a}")
