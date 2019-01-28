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

Generate some simple test data (creates by default 'newdata.dat'):
```
$ ./gen
```

Run test program (uses by default 'newdata.dat'):
```
$ ./binary_search
```

If you compiled with google benchmark support (file name 'newdata.dat' hardcoded atm):
```
$ ./binary_search_bench
```


## Techniques for Interleaving

Group Prefetching
https://doi.acm.org/10.1145/1272743.1272747

Asynchronous Memory Access Chaining
http://www.vldb.org/pvldb/vol9/p252-kocberger.pdf

Interleaving with Coroutines
http://www.vldb.org/pvldb/vol11/p230-psaropoulos.pdf
