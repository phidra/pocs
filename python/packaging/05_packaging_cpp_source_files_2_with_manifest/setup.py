import setuptools


print(
    """
This POC shows another way to package *.cpp/*.h files, by using a MANIFEST.in file

NOTE : this is NOT a POC on a python C/C++ extension.
This is merely a POC to demonstrate how to tweak python's packaging in order to embed cpp source files.
"""
)


setuptools.setup(
    name="mysupercpplib",
    version="0.0.1",
    author="It's a me",
    author_email="itsame@example.com",
    description="A super cpp lib",
    packages=setuptools.find_packages(),
)
