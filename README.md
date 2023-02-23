# cuckoo\_hash - C implementation of cuckoo hashing.


### REQUIREMENTS

- cmake
- a working C compiler that support C99


### BUILD AND INSTALL

```
$ mkdir build && cd build
$ cmake ..
$ make
# make install
```

### CMAKE OPTIONS:

- `CUCKOO_BUILD_STATIC` (ON or OFF to enable/disable static library, default OFF)
- `CUCKOO_BUILD_SHARED` (ON or OFF to enable/disable shared library, default ON)
- `CUCKOO_MIN_CAP` (set minimum hash table capacity, default 16)
- `CUCKOO_MAX_CAP` (set maximum hash table capacity, default 8092)
- `CMAKE_C_COMPILER` (specify C compiler, default cc)
- `CMAKE_INSTALL_PREFIX` (specify installation directory, default /usr/local)
- `CUCKOO_ENABLE_ASAN` (ON or OFF to enable/disable AddressSanitizer, default OFF)
- `CUCKOO_ENABLE_TSAN` (ON or OFF to enable/disable ThreadSanitizer, default OFF)
- `CUCKOO_BUILD_EXAMPLE` (ON or OFF to build example files, default ON)


### NOTES

- To know whether an entry is occupied or not, I use entry `key` and
  compare it with NULL, if NULL then it's not occupied and insert
  new entry there.
- Fixed-size, it doesn't grow nor rehash.


### LINKS
- https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.25.4189&rep=rep1&type=pdf
- http://www.lkozma.net/cuckoo_hashing_visualization
- https://en.wikipedia.org/wiki/Cuckoo_hashing
- https://programming.guide/cuckoo-hashing.html
