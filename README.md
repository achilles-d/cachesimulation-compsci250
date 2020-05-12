# Homework 5: Cache Simulation
## ***COMPSCI250: Computer Architecture***
#### Note: z2017g is my old GitHub account that was used for schoolwork. 
## C Files
### virt2phys.c
* Outputs a given virtual address's corresponding physical address in hex.
#### Usage: 
~~~c
./virt2phys <page-table-file> <virtual-address>
~~~
##### Arguments
~~~c
<page-table-file>
~~~
File containing page table (full path to file).
~~~c
<virtual-address>
~~~~
Virtual address to translate (in hex).
### cachesim.c
* Simulates a single-level 1MB 4-way set-associative cache that uses LRU replacement.
* The cache is write back and write allocate. 
* The block size is 32B.
#### Usage: 
~~~c
./cachesim <trace-file> <cache-size> <associativity> <block-size>
~~~
##### Arguments
~~~c
<trace-file>
~~~
Filename (full path to file) of the memory access trace file.
~~~c
<cache-size>
~~~~
Total capacity of the cache (KB). 
~~~c
<associativity>
~~~~
Set associativity of the cache. 
~~~c
<block-size>
~~~~
The size of the cache blocks (B).

