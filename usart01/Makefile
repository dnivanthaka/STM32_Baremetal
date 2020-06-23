# Makefile for blink demo

#TOOLS = arm-linux-gnu
TOOLS = arm-none-eabi

# Assembling with gcc makes it want crt0 at link time.
#AS = $(TOOLS)-gcc
AS = $(TOOLS)-as
# Use the -g flag if you intend to use gdb
#CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb, added -Os flag to optimize code generation
CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb -g -Os

#LD = $(TOOLS)-gcc
LD = $(TOOLS)-ld.bfd
OBJCOPY = $(TOOLS)-objcopy
DUMP = $(TOOLS)-objdump -d
GDB = $(TOOLS)-gdb

OBJS = start.o gpio.o rcc.o usart.o

all: usart.bin usart.dump

usart.dump:	usart.elf
	$(DUMP) usart.elf >usart.dump

usart.bin:	usart.elf
	$(OBJCOPY) usart.elf usart.bin -O binary

usart.elf: 	$(OBJS)
	$(LD) -T usart.lds -o usart.elf $(OBJS)

start.o:	start.s
	$(AS) start.s -o start.o

usart.o:	usart.c
	$(CC) -c usart.c

gpio.o:	gpio.c
	$(CC) -c gpio.c

rcc.o:	rcc.c
	$(CC) -c rcc.c

clean:
	rm -f *.o usart.elf usart.bin usart.dump
