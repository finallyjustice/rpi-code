CROSS_PREFIX= arm-none-eabi-

AS = $(CROSS_PREFIX)as
CC = $(CROSS_PREFIX)gcc
LD = $(CROSS_PREFIX)ld
OBJCOPY = $(CROSS_PREFIX)objcopy
OBJDUMP = $(CROSS_PREFIX)objdump

CFLAGS = -Wall -O0 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s
LDFLAGS = -L . -lgcc

OBJS = \
	main.o\
	uart.o

all : kernel.img

entry.o : entry.S
	$(AS) entry.S -o entry.o

kernel.img : $(OBJS) kernel.ld entry.o uart.o main.o
	$(LD) entry.o $(OBJS) -T kernel.ld -o kernel.elf $(LDFLAGS)
	$(OBJDUMP) -D kernel.elf > kernel.asm
	$(OBJCOPY) kernel.elf -O binary kernel.img

clean:
	rm -f *.o *.elf *.asm *.img
