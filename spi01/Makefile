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

OBJS = start.o gpio.o rcc.o usart.o spi.o

all: spi.bin spi.dump

spi.dump:	spi.elf
	$(DUMP) spi.elf >spi.dump

spi.bin:	spi.elf
	$(OBJCOPY) spi.elf spi.bin -O binary

spi.elf: 	$(OBJS)
	$(LD) -T spi.lds -o spi.elf $(OBJS)

start.o:	start.s
	$(AS) start.s -o start.o

spi.o:	spi.c
	$(CC) -c spi.c

usart.o:	usart.c
	$(CC) -c usart.c

gpio.o:	gpio.c
	$(CC) -c gpio.c

rcc.o:	rcc.c
	$(CC) -c rcc.c

clean:
	rm -f *.o spi.elf spi.bin spi.dump
