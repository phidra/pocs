import setuptools

# Ensure setuptools is recent enough to use setup.cfg
setuptools_major = int(setuptools.__version__.split(".")[0])
assert setuptools_major >= 30, "setuptools is too old to use setup.cfg -> run POC in a virtualenv"

setuptools.setup()
