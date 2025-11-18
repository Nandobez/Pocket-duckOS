#
# Makefile for DuckOS.
# Version : 0.0.01
#

CC = gcc
CFLAGS = -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra -c

LDFLAGS = -m elf_i386 -s -x -M -T boot/link.ld

AS = nasm
ASFLAGS = -f elf32

%.o: %.c
	$(CC) $(CFLAGS) -I include $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

all:	duckos/bin

duckos/bin:lib/lib kernel/kernel init/init mm/mm tools/tools features/features system

tools/tools: tools/shell.o

features/features:
	(cd features;make)

lib/lib:
	(cd lib;make)

mm/mm:
	(cd mm;make)

kernel/kernel: kernel/asm.o kernel/console.o kernel/desc.o \
	kernel/exit.o kernel/keyboard.o kernel/mktime.o \
	kernel/panic.o kernel/printk.o kernel/traps.o \
	kernel/tty_io.o kernel/vsprintf.o kernel/sched.o

init/init: init/main.o

system: boot/boot.o
	ld $(LDFLAGS) \
	-o duckos.bin \
	lib/*.o kernel/*.o init/*.o boot/*.o mm/*.o tools/*.o features/*.o > system.map

clean:
	(cd lib;make clean)
	(cd mm;make clean)
	(cd features;make clean)
	rm -rf kernel/*.o init/*.o boot/*.o tools/*.o
	rm -rf duckos.bin system.map *~
	rm -rf kernel/*~ init/*~ boot/*~ tools/*~
	rm -rf include/*~ include/asm/*~ include/sys/*~ include/duckos/*~

backup: clean
	(cd ..;tar -zcvf duckos.tar.gz duckos)

run:
	qemu-system-i386 -kernel duckos.bin
