KernelMQ
========

Message queue with additional IPC capabilities implemented as multiboot microkernel.

Glossary
--------

* `addr` - physical address (`unsigned long`)
* `base` - physical or virtual start address (`unsigned long`)
* `limit` - physical or virtual end address (included, `unsigned long`)
* `size` - size in bytes (`unsigned long`)
* `length` - number of entries (`unsigned int`)

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
./configure build
make clean
make run
```

Run tests
---------

```sh
./configure test
make clean
make test
```
