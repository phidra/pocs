from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

example_module = Pybind11Extension(
    "pybind11_stl_structures",
    [
        "src/operations.cpp",
    ],
    include_dirs=["include"],
    extra_compile_args=["-O3"],
)

setup(
    name="pybind_11_stl",
    version=0.1,
    author="",
    author_email="",
    ext_modules=[example_module],
    cmdclass={"build_ext": build_ext},
)
