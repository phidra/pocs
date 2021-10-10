#!/usr/bin/env python3

# By nature (inexisting attributes, importing files long after the beginning), this file is not pep8 compliant :
# flake8: noqa
# type: ignore


print(
    """

    This POC shows that a package is nothing more than a module with a '__path__' attribute :

    - the POC STATICALLY defines a 'greeter' module ; 'greeter' is a regular module, it is NOT a package
        + we show that as a module, greeter has no '__path__' attribute
        + thus, 'import greeter.poc_python_import08' fails accordingly
    - we DYNAMICALLY add a '__path__' attribute to 'greeter', that contains `/tmp`
    - moreover, we create a custom module `/tmp/poc_python_import08.py` that contains dummy code
    - after doing that, we CAN import and use /tmp/poc_python_import08.py as a SUBMODULE of the 'greeter' package !

    TL;DR : some_package.__path__ acts kind of like an additional sys.path entry.

    Reference :
        https://docs.python.org/3/reference/import.html#module-path
"""
)

# importing and using 'greeter' module :
import greeter

greeter.greet()

# showing that module object 'greeter' has no '__path__' attribute :
assert not hasattr(greeter, "__path__")

# showing that 'greeter' is not a package (at least not containing a 'poc_python_import08' submodule) :
try:
    import greeter.poc_python_import08
except ModuleNotFoundError:
    print("KABOOM 2 = cannot import 'greeter.poc_python_import08'")

# note : the POC is more readable with a static filename 'poc_python_import08.py' than a dynamic one:
# creating our custom module :
with open("/tmp/poc_python_import08.py", "w") as f:
    f.write('print("IMPORTED")\n')
    f.write("def say_pouetpouet():\n")
    f.write('    print("What I am saying is : pouetpouet")\n')

# turning 'greeter' into a package, by adding a '__path__' attribute, so that our custom-module is importable :
greeter.__path__ = ["/tmp"]

# our custom module is now importable as a submodule of the 'greeter' package !
import greeter.poc_python_import08

print("OK : 'greeter.poc_python_import08' was imported !")

# moreover, we can use it :
greeter.poc_python_import08.say_pouetpouet()
