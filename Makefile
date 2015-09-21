all: kernel.bin

kernel.bin: start.o main.o scrn.o gdt.o idt.o isrs.o irq.o timer.o kb.o
	/home/alex/os/kernel/i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-ld -T link.ld -o kernel.bin start.o main.o scrn.o gdt.o idt.o isrs.o irq.o timer.o kb.o

start.o: start.asm
	nasm -f elf32 -o start.o start.asm

main.o: main.c
	/home/alex/os/kernel/i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -std=c99 -fno-builtin -I./include -c -o main.o main.c

scrn.o: scrn.c
	/home/alex/os/kernel/i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -std=c99 -fno-builtin -I./include -c -o scrn.o scrn.c

gdt.o: gdt.c
	/home/alex/os/kernel/i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -std=c99 -fno-builtin -I./include -c -o gdt.o gdt.c

idt.o: idt.c
	/home/alex/os/kernel/i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -std=c99 -fno-builtin -I./include -c -o idt.o idt.c

isrs.o: isrs.c
	/home/alex/os/kernel/i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -std=c99 -fno-builtin -I./include -c -o isrs.o isrs.c

irq.o: irq.c
	/home/alex/os/kernel/i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -std=c99 -fno-builtin -I./include -c -o irq.o irq.c

timer.o: timer.c
	/home/alex/os/kernel/i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -std=c99 -fno-builtin -I./include -c -o timer.o timer.c

kb.o: kb.c
	/home/alex/os/kernel/i686-elf-4.9.1-Linux-x86_64/bin/i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -std=c99 -fno-builtin -I./include -c -o kb.o kb.c

.PHONY: clean
clean:
	rm -rf *.o *.bin
