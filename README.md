# C Data Structures

Generic container data structures in C.

```bash
$ make
$ ./bin/main input/grafo_1.txt path 1 4

d(1, 4) = 25.000000, [4, 3, 2, 1]

$ ./bin/main input/grafo_1.txt path 1 .

d(1, 2) = 15.000000, [2, 1]
d(1, 3) = 17.000000, [3, 2, 1]
d(1, 4) = 25.000000, [4, 3, 2, 1]
[...]
d(1, 98) = 19.000000, [98, 99, 100, 1]
d(1, 99) = 16.000000, [99, 100, 1]
d(1, 100) = 12.000000, [100, 1]

$ ./bin/main input/grafo_1.txt mst

336.000000
2 -> 37
3 -> 2
4 -> 95
[...]
99 -> 100
100 -> 1
```
