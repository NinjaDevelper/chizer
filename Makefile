all: chizer
chizer: cxx/chizer.c cxx/chizer.h
	gcc -Wall -DCHI_MAIN -o cxx/chizer cxx/chizer.c -O3
	
test: cxx/chizer.c tests/test.c cxx/chizer.h
	cd libtap; make
	gcc  -fprofile-arcs -ftest-coverage -Wall -DDEBUG -o test tests/test.c cxx/chizer.c -Icxx -ltap -Ilibtap -Llibtap

clean:
	rm *.o test *.so
