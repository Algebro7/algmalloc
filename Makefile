CC=gcc 
CFLAGS=-Wall -m32
LDFLAGS= -Wl,-rpath,'.'

all: algmalloc-test

algmalloc-test: algmalloc-test.o libalgmalloc.so
	$(CC) $(LDFLAGS) $(CFLAGS) algmalloc-test.o -o algmalloc-test libalgmalloc.so

algmalloc-test.o: algmalloc-test.c
	$(CC) $(CFLAGS) -O -c algmalloc-test.c

libalgmalloc.so: algmalloc.c
	$(CC) $(CFLAGS) -shared -fPIC -o libalgmalloc.so algmalloc.c

clean:
	rm algmalloc-test *.o *.so