obj-m := add_mod/ avg_mod/

KDIR=/lib/modules/$(shell uname -r)/build/

PWD=$(shell pwd)

all:
		make -C $(KDIR) M=$(PWD) modules

install:
		make -C $(KDIR) M=$(PWD) modules_install
		depmod -a

clean:
		make -C $(KDIR) M=$(PWD) clean
		rm -rf /lib/modules/$(shell uname -r)/extra/*
		depmod -a