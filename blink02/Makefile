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

OBJS = start.o gpio.o rcc.o blink.o

all: blink.bin blink.dump

blink.dump:	blink.elf
	$(DUMP) blink.elf >blink.dump

blink.bin:	blink.elf
	$(OBJCOPY) blink.elf blink.bin -O binary

blink.elf: 	$(OBJS)
	$(LD) -T blink.lds -o blink.elf $(OBJS)

start.o:	start.s
	$(AS) start.s -o start.o

blink.o:	blink.c
	$(CC) -c blink.c

gpio.o:	gpio.c
	$(CC) -c gpio.c

rcc.o:	rcc.c
	$(CC) -c rcc.c

clean:
	rm -f *.o blink.elf blink.bin blink.dump
