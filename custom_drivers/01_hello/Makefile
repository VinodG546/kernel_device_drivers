obj-m := 01_hello_world.o
KDIR := /home/vinod/workspace/ldd/source/linux_6.16/KERNEL
PWD := $(shell pwd)

CROSS_COMPILE := arm-linux-gnueabihf-

all:
	make -C $(KDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

