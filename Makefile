
.PHONY: all clean install run

run: install

	qemu-system-i386 -S -s -boot a -fda a.img

install: boot1 boot2 a.img

	# format image
	mformat a:

	# copy bootloader to image
	dd if=boot1 of=a.img bs=1 count=512 conv=notrunc

	# copy OS to image
	mcopy -o boot2 a:BOOT2

clean:

	rm *.o *.elf boot1 boot2 boot1.list a.img

a.img:
	
	# create virtual floppy
	bximage -fd -size=1.44 -q a.img

boot1: boot2 ./src/asm/boot1.asm

	# build stage 1 OS
	nasm -l boot1.list -DENTRY=`./bin/getaddr.sh main` ./src/asm/boot1.asm
	mv ./src/asm/boot1 ./boot1

boot2: boot2.elf

	# strip unneeded things
	objcopy -S -O binary boot2.elf boot2 

boot2.elf: boot2_c.o boot2_S.o ready_queue.o interrupts.o

	# link files
	ld -g -melf_i386 -Ttext 0x10000 -e main -o boot2.elf boot2_c.o boot2_S.o ready_queue.o interrupts.o

boot2_c.o: ./src/c/boot2.c

	# build object files for stage 2 OS
	gcc -g -m32 -c -std=c11 -o boot2_c.o ./src/c/boot2.c

boot2_S.o: ./src/asm/boot2.S

	gcc -g -m32 -c -masm=intel -o boot2_S.o ./src/asm/boot2.S

ready_queue.o: ./src/c/ready_queue.c

	gcc -g -m32 -c -std=c11 -o ready_queue.o ./src/c/ready_queue.c	

interrupts.o: ./src/c/interrupts.c

	gcc -g -m32 -c -std=c11 -o interrupts.o ./src/c/interrupts.c

