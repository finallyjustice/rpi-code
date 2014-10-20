CROSS_PREFIX= arm-none-eabi-

AS = $(CROSS_PREFIX)as
CC = $(CROSS_PREFIX)gcc
LD = $(CROSS_PREFIX)ld
OBJCOPY = $(CROSS_PREFIX)objcopy
OBJDUMP = $(CROSS_PREFIX)objdump

CFLAGS = -Wall -O0 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s
LDFLAGS = -L . -lgcc

OBJS = \
	secure_entry.o \
	secure_start.o \
	secure_uart.o \
	normal_entry.o \
	normal_start.o \

all : kernel.img

kernel.img : kernel.ld $(OBJS) 
	$(LD) $(OBJS) -T kernel.ld -o kernel.elf $(LDFLAGS)
	$(OBJDUMP) -D kernel.elf > kernel.asm
	$(OBJDUMP) -t kernel.elf | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > kernel.sym
	$(OBJCOPY) kernel.elf -O binary kernel.img

clean :
	rm -f *.o *.elf *.asm *sym *.img 
