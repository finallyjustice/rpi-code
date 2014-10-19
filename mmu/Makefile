CROSS_PREFIX= arm-none-eabi-

AS = $(CROSS_PREFIX)as
CC = $(CROSS_PREFIX)gcc
LD = $(CROSS_PREFIX)ld
OBJCOPY = $(CROSS_PREFIX)objcopy
OBJDUMP = $(CROSS_PREFIX)objdump

CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 
LDFLAGS = -L . -lgcc

OBJS = \
	start.o \
	early.o \
	asm.o \
	main.o \
	uart.o \

all : kernel.img

entry.o : entry.S
	$(AS) entry.S -o entry.o

kernel.img : kernel.ld entry.o $(OBJS)
	$(LD) entry.o $(OBJS) -T kernel.ld -o kernel.elf $(LDFLAGS)
	$(OBJDUMP) -D kernel.elf > kernel.asm
	$(OBJDUMP) -t kernel.elf | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > kernel.sym
	$(OBJCOPY) kernel.elf -O binary kernel.img

clean :
	rm -f *.o *.elf *.asm *.sym *.img 
