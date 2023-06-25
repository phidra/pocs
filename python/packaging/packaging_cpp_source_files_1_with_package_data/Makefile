all: clean build check info

clean:
	rm -rf dist/
	rm -rf *.egg-info/

build:
	python3 setup.py sdist

check:
	tar -tvzf dist/*.tar.gz 

info:
	echo ""
	echo ""
	echo "You should see that src/lib.cpp and src/lib.h are included in generated package."
