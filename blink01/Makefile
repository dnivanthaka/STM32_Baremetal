# Makefile for blink demo

#TOOLS = arm-linux-gnu
TOOLS = arm-none-eabi

# Assembling with gcc makes it want crt0 at link time.
#AS = $(TOOLS)-gcc
AS = $(TOOLS)-as
# Use the -g flag if you intend to use gdb
#CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb
CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb -g

#LD = $(TOOLS)-gcc
LD = $(TOOLS)-ld.bfd
OBJCOPY = $(TOOLS)-objcopy
DUMP = $(TOOLS)-objdump -d
GDB = $(TOOLS)-gdb

OBJS = start.o blink.o

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

clean:
	rm -f *.o blink.elf blink.bin blink.dump
