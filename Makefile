all: chizer
chizer: chizer.c chizer.h
	gcc -Wall -DCHI_MAIN -o chizer chizer.c -O3
test: chizer.c test.c chizer.h
	cd libtap; make
	gcc  -fprofile-arcs -ftest-coverage -static -Wall -DDEBUG -o test test.c chizer.c -ltap -Ilibtap -Llibtap
python: chizer.c chizer.h chizer_python.c setup.py
	python setup.py build_ext -i
clean:
	rm *.o test *.so
