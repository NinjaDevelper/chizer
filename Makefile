all: chizer

chizer: cxx/chizer.c
	$(CC) -Wall -DCHI_MAIN -o cxx/chizer cxx/chizer.c -O3
	
test: cxx/chizer.o tests/test.o
	cd libtap; make
	$(CC) -fprofile-arcs -ftest-coverage -Wall -DDEBUG -o test $^ -ltap -Llibtap

.c.o:
	$(CC) -fprofile-arcs -ftest-coverage -Wall -DDEBUG -c $^ -o $@ -Icxx -Ilibtap

clean:
	rm -f *.o
	rm -f cxx/*.o
	rm -f tests/*.o
	rm -f test
	rm -f *.so
