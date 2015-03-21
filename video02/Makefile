
ARMGNU ?= arm-none-eabi

COPS = -Wall -g -O2 -std=gnu11 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-a7

gcc : kernel.img

all : gcc #clang

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.s


vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

video02.o : video02.c
	$(ARMGNU)-gcc $(COPS) -c video02.c -o video02.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

kernel.img : loader vectors.o periph.o video02.o 
	$(ARMGNU)-ld vectors.o periph.o video02.o -T loader -o video02.elf
	$(ARMGNU)-objdump -D video02.elf > video02.list
	$(ARMGNU)-objcopy video02.elf -O ihex video02.hex
	$(ARMGNU)-objcopy video02.elf -O binary kernel.img

install : gcc
	rm -f /Volumes/RPI/kernel.img
	cp -v kernel.img /Volumes/RPI/kernel.img
	diskutil eject RPI

rebuild : clean all

redo : clean install
