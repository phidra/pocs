from setuptools import setup, Extension

pouet = Extension(
    "pouet",
    ["pouetmodule.c"],
)

setup(
    name="cextpoc",
    version=0.1,
    author="phidra",
    author_email="phidra@example.com",
    url="https://docs.python.org/3/extending/extending.html",
    description="POC on C extension",
    ext_modules=[pouet],
)
