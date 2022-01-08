obj-m := chardev4.o

KDIR=/lib/modules/$(shell uname -r)/build/

PWD=$(shell pwd)

all:
		make -C $(KDIR) M=$(PWD) modules
		gcc -o add add.c
		gcc -o sub sub.c
		

install:
		make -C $(KDIR) M=$(PWD) modules_install

clean:
		make -C $(KDIR) M=$(PWD) clean