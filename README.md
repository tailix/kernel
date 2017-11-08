KernelMQ
========

Message queue with additional IPC capabilities implemented as multiboot microkernel.

Glossary
--------

* `addr` - aligned physical address, shifted right with number of zero bits (`unsigned long`)
* `base` - physical or virtual start address (`unsigned long`)
* `limit` - physical or virtual end address (included, `unsigned long`)
* `size` - size in bytes (`unsigned long`)
* `length` - number of entries (`unsigned int`)
* `slen` - string length excluding the terminating null byte (`unsigned int`)

Dependencies
------------

### Build youself:

* Cross-compiler for i686-elf

### Debian 9.2 packages:

* make
* grub-pc-bin
* qemu-system-x86

Build and run
-------------

```sh
./configure build-x86
make clean
make run
```

Run tests
---------

```sh
./configure test-host
make clean
make test
```
