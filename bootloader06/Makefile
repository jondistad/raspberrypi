
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : kernel.img blinker.bin

all : gcc

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

bootloader06.o : bootloader06.c
	$(ARMGNU)-gcc $(COPS) -c bootloader06.c -o bootloader06.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

kernel.img : loader vectors.o periph.o bootloader06.o 
	$(ARMGNU)-ld vectors.o periph.o bootloader06.o -T loader -o bootloader06.elf
	$(ARMGNU)-objdump -D bootloader06.elf > bootloader06.list
	$(ARMGNU)-objcopy bootloader06.elf -O ihex bootloader06.hex
	$(ARMGNU)-objcopy bootloader06.elf -O binary kernel.img





start.o : start.s
	$(ARMGNU)-as start.s -o start.o

blinker.o : blinker.c
	$(ARMGNU)-gcc $(COPS) -c blinker.c -o blinker.o

blinker.bin : memmap start.o blinker.o 
	$(ARMGNU)-ld start.o blinker.o -T memmap -o blinker.elf
	$(ARMGNU)-objdump -D blinker.elf > blinker.list
	$(ARMGNU)-objcopy blinker.elf -O ihex blinker.hex
	$(ARMGNU)-objcopy blinker.elf -O binary blinker.bin
