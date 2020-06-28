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

OBJS = start.o gpio.o rcc.o usart.o i2c.o

all: i2c.bin i2c.dump

i2c.dump:	i2c.elf
	$(DUMP) i2c.elf >i2c.dump

i2c.bin:	i2c.elf
	$(OBJCOPY) i2c.elf i2c.bin -O binary

i2c.elf: 	$(OBJS)
	$(LD) -T i2c.lds -o i2c.elf $(OBJS)

start.o:	start.s
	$(AS) start.s -o start.o

i2c.o:	i2c.c
	$(CC) -c i2c.c

usart.o:	usart.c
	$(CC) -c usart.c

gpio.o:	gpio.c
	$(CC) -c gpio.c

rcc.o:	rcc.c
	$(CC) -c rcc.c

clean:
	rm -f *.o i2c.elf i2c.bin i2c.dump
