#!/usr/bin/env python3

# By nature (inexisting attributes, importing files long after the beginning), this file is not pep8 compliant :
# flake8: noqa

print(
    """

    This POC demonstrates a rather unintuitive fact (at least for me) : importing a package DOES NOT import
    its children modules or subpackages.

    Instead, impporting a package merely execute the package's __init__.py, and only it !

    Moreover, we CANNOT use the children modules or subpackage from the imported package ! We have to import them
    explicitly.

    What the POC does :
        - it defines a 'root_package', which has a 'subpackage', and a 'child_module'
        - when imported, everyone (root_package, subpackage, child_module) prints a message
        - step 1 = we begin by only importing the root_package :
            + we see that ONLY the print message of the root_package is displayed
            + notably, the print messge of child_module and subpackage are NOT displayed
            + this shows that they are not implicitly imported when we import the root_package !
        - step 2 = thus, we cannot use subpackage/child_module : 'root_package.child_module' raises AttributeError
        - step 3 = to be able to use child_module/subpackage, we must EXPLICITLY "import root_package.child_module"
            + of course, we then finally see their print message, and we can now use 'root_package.child_module'
"""
)

import root_package

# step 1 =using the root_package is immediatly possible after the import of the root_package (obviously) :
root_package.root_package_function()

# step2 = using the subpackage, or the child_module is NOT possible :
try:
    root_package.subpackage.subpackage_function()
except AttributeError:
    print("KABOOM 1 : we cannot use subpackage here")
try:
    root_package.child_module.child_module_function()
except AttributeError:
    print("KABOOM 2 : we cannot use child_module here")

# step 3 = we must EXPLICITLY import the subpackage / child_module to be able to use them :
import root_package.subpackage

root_package.subpackage.subpackage_function()

import root_package.child_module

root_package.child_module.child_module_function()
