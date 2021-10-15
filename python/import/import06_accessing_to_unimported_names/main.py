#!/usr/bin/env python3

import sys
from typing import Any

# we only import the name 'do_greet' from the module 'greeter':
from greeter import do_greet

# BUT as we will see, the whole module (and all its content, even the unimported objects) was loaded.

print(
    """
    This POC shows that even if we only imported 'do_greet', the whole 'greeter' module was loaded.
    With overly complicated access of loaded modules, we indeed can use other members of the modules.

    The POC :
        - loads the function 'do_greet' from the module 'greeter'
        - shows that we can't directly use the parent module 'greeter' (as we only imported its name 'do_greet')
        - shows that we can still indirectly access to the loaded module, and use its unimported names
"""
)

do_greet("pouet1")
do_greet("pouet2", "fr_FR")
do_greet("pouet3", "it_IT")


# we see that 'greeter' module was not imported in the current module namespace :
try:
    print(len(greeter.I18N_GREETINGS))  # type: ignore
except NameError:
    print("Unable to use 'greeter' module directly...")


# BUT the module was still loaded, and we can access it, although in an unpractical way.
# Here, we get the module that holds 'do_greet' :
greeter_module: Any = sys.modules[do_greet.__module__]
print("I know how to greet in the following languages :")
for language_code in greeter_module.I18N_GREETINGS:
    print(language_code)
print("")
