import setuptools

setuptools.setup(
    name="basic1",
    version="0.0.1",
    author="Luke SKYWALKER",
    author_email="luke.skywalker@rebellion.universe",
    description="POC on basic packaging",
    package_dir={"usefulmodule": "src"},
    packages=["usefulmodule"],
)
