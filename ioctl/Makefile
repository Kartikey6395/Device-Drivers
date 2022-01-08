obj-m := char_drv_ioctl.o

KDIR=/lib/modules/$(shell uname -r)/build/

PWD=$(shell pwd)

all:
		make -C $(KDIR) M=$(PWD) modules
		gcc -o uconfig uconfig.c
		
install:
		make -C $(KDIR) M=$(PWD) modules_install

clean:
		make -C $(KDIR) M=$(PWD) clean