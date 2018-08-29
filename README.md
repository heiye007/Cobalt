# Cobalt

![Cobalt](https://cakehonolulu.github.io/assets/img/cobalticon.png "Cobalt Icon")

## What is this?

Cobalt is an 32-Bit kernel made from scratch using C11 and i386 Assembly (Using AT&T syntax).

## Project goals

POSIX compatibility and feature-rich.

## Compiling

Cobalt compiling structure will search for a "CROSS_COMPILE" environment variable pointing to the location where your Cross Compiler resides plus it's designation:

```
export CROSS_COMPILE=/home/user/My_cross_compiler/bin/i686-elf-
```

And another environment variable designing the architecture you want to build Cobalt for:

```
export ARCH=i386
```

At the end of the compilation you'll get a GRUB iso (Named kernel.iso) which you can use to boot Cobalt.