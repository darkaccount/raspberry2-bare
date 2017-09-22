
ARMGNU ?= arm-none-eabi
COPS = -Wall -O0 -nostdlib -nostartfiles -ffreestanding 
OCOPS = -O0 -DRPI2 -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -nostartfiles -g 
LLCOPS =  -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 

all : kernel.elf kernel.img 

vectors.o : vectors.s
	$(ARMGNU)-as $^ -o $@
	
001.o : 001.c
	$(ARMGNU)-gcc $(COPS) -c $^

kernel.elf : memmap vectors.o 001.o
	$(ARMGNU)-ld vectors.o 001.o -T $< -o $@ 
	$(ARMGNU)-objdump -D $@ > kernel.asm 
	$(ARMGNU)-nm $@ > kernel.nm

kernel.img : kernel.elf
	$(ARMGNU)-objcopy $^ -O binary $@

clean : 
	rm -f *.o
	rm -f *.elf
	rm -f *.img
	rm -f *.nm
	rm -f *.asm

disa : kernel.elf 
	$(ARMGNU)-objdump -D $^ > kernel.asm 
	$(ARMGNU)-nm $^ > kernel.nm