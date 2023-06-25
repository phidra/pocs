import setuptools


print("""
This POC shows a possible mean to package *.cpp/*.h files, by using `package_data` attribute.

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
    package_data={"": ["*.cpp", "*.h"]},
)
