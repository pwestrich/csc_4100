# csc_4100
My small operating system from its course. 

You need several tools installed in order to build it. Most notable are GCC, Nasm, objdump, and quemu for running. The makefile should list everything needed somewhere in it; you may have to look. As far as I know it should build and run on any Linux distro with the tools installed. 

In this state all it should do is boot up, clear the screen, and have a few processes use some semaphores and mutexes.
