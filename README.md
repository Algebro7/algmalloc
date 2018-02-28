# algmalloc
`algmalloc` is a naive implementation of `malloc` and `free` as an exercise in C and Linux system programming.
## Compiling and Running
First, build the shared library:  
`clang -shared -fPIC -o libalgmalloc.so algmalloc.c -m32 -Wall`  
  
Next, build the test harness and link to libalgmalloc:  
`clang algmalloc-test.c -o algmalloc-test libalgmalloc.so -Wl,-rpath,/home/algebro/path/to/algmalloc -Wall -m32`  
  
Run the test harness:  
`./algmalloc-test`
