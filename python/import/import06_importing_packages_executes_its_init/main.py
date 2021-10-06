#!/usr/bin/env python3

# the import order matters here, for the comment to be relevant :
import parent.three
import parent.two
import parent.one

print(
    """

    This POC is the direct application of what is explained here :
        https://docs.python.org/3/reference/import.html#regular-packages

    More specifically, the question is "when a package's __init__.py is executed ?".

    NOTE : a package is just a module with additional pecularities. For a package, 'executing a module' means executing
           its '__init__.py'. Moreover, as modules, packages are also executed only once, and cached in sys.modules.

    We can see that :
        - the module 'parent' is never explicitly imported
        - 'parent/__init__.py' is still executed, when parent is IMPLICITLY imported by 'parent.three'
        - (and just after that, 'parent/three/__init__.py' is also executed)
        - for the other imports two and one, parent is ALSO implicitly imported, but as the 'parent' module was already
          imported, 'parent/__init__.py' is not executed another time : the cache (sys.modules) is used instead.

    EXPECTED OUTPUT :
        === If you see this message, parent/__init__.py was executed
        === If you see this message, parent/three/__init__.py was executed
        === If you see this message, parent/two/__init__.py was executed
        === If you see this message, parent/one/__init__.py was executed
        [...]
        greetings from parent.one !
        greetings from parent.two !
        greetings from parent.three !
"""
)

parent.one.do_greet()
parent.two.do_greet()
parent.three.do_greet()
