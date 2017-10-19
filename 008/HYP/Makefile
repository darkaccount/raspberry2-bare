
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : kernel.img 

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.srec
	rm -f *.img

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

src.o : src.c
	$(ARMGNU)-gcc $(COPS) -c src.c -o src.o

periph.o : periph.c 
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

kernel.img : loader vectors.o periph.o src.o 
	$(ARMGNU)-ld vectors.o periph.o src.o -T loader -o src.elf
	$(ARMGNU)-objdump -D src.elf > src.list
	$(ARMGNU)-objcopy src.elf -O ihex src.hex
	$(ARMGNU)-objcopy src.elf -O srec --srec-forceS3 src.srec
	$(ARMGNU)-objcopy src.elf -O binary kernel.img


