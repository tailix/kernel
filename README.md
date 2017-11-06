KernelMQ
========

Message queue with additional IPC capabilities implemented as multiboot microkernel.

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
