KernelMQ
========

Message queue with additional IPC capabilities implemented as multiboot microkernel.

Glossary
--------

* (`unsigned long`) `addr`   - aligned physical address, shifted right with number of zero bits
* (`unsigned long`) `base`   - physical or virtual start address
* (`unsigned long`) `limit`  - physical or virtual end address (included)
* (`unsigned long`) `size`   - size in bytes
* (`unsigned int`)  `length` - number of entries
* (`unsigned int`)  `slen`   - string length excluding the terminating null byte
