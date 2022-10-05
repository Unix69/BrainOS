#
# Copyright (C) 2018 bzt (bztsrc@github)
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#
#

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

ASMSRCS = $(wildcard *.S)
ASMOBJS = $(ASMSRCS:.S=.o)


CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
CC = aarch64-linux-gnu-gcc
LD = aarch64-linux-gnu-ld
OBJCPY = aarch64-linux-gnu-objcopy
RUN = qemu-system-aarch64
TARGET = kernel8.img
LINKMAP = link.ld
KERNEL = kernel.elf

all: clean $(TARGET)


%.o: %.S
	$(CC) $(CFLAGS) -c  $< -o $@
	
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(ASMOBJS) $(OBJS)
	$(LD) -nostdlib -nostartfiles $(ASMOBJS) $(OBJS) -T $(LINKMAP) -o $(KERNEL)
	$(OBJCPY) -O binary $(KERNEL) $(TARGET)

clean:
	rm $(KERNEL) *.o >/dev/null 2>/dev/null || true

run:
	sudo $(RUN) -M virt -machine type=raspi3,accel=tcg,kernel_irqchip=on -cpu cortex-a53 -smp 4 -kernel $(TARGET) -serial stdio
