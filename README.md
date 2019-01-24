# interleaved-algorithms
testbed for instruction stream interleaving

binary search is the only algorithm at the moment

## Run

Compilation (optionally depends on google benchmark library):
```
$ mkdir build
$ cmake ..
$ make
```

Generate some simple test data:
```
$ ./gen newdata.dat
```

Run test program:
```
$ ./binary_search newdata.dat
```

If you compiled with google benchmark support:
```
$ ./binary_search_bench
```
