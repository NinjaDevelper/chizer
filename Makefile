all: chizer
chizer: cxx/chizer.c cxx/chizer.h
	gcc -Wall -DCHI_MAIN -o cxx/chizer cxx/chizer.c -O3
	
test: cxx/chizer.c tests/test.c cxx/chizer.h
	cd libtap; make
	gcc  -fprofile-arcs -ftest-coverage -static -Wall -DDEBUG -o test tests/test.c cxx/chizer.c -Icxx -ltap -Ilibtap -Llibtap

python: cxx/chizer.c cxx/chizer.h cxx/chizer_python.c setup.py
	python setup.py build_ext -i
clean:
	rm *.o test *.so
