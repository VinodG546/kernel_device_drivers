obj-m := driver.o
KDIR := /home/vinod/workspace/ldd/source/bb-kernel/KERNEL
PWD := $(shell pwd)
ARCH := arm
CROSS_COMPILE := arm-none-linux-gnueabihf-

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
host:
	make -C /lib/modules/`uname -r`/build M=$(PWD) modules
