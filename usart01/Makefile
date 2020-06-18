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

# To burn the image:
#  Connect the STLINK gadget to the target.
#  Be sure and start openOCD in this directory.
#    (or else it won't be able to find the bin file).
#  Then use "make flash" to start an openocd console session.
#  Type: reset halt
#  Type: flash write_image erase blink.bin 0x08000000
flash:
	@echo "type: flash write_image erase blink.bin 0x08000000"
	telnet localhost 4444
gdb:
	$(GDB) --eval-command="target remote localhost:3333" blink.elf

gdbtui:
	$(GDB) -tui --eval-command="target remote localhost:3333" blink.elf

clean:
	rm -f *.o usart.elf usart.bin usart.dump
