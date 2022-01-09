obj-m := chrdrv_tasklet.o

KDIR=/lib/modules/$(shell uname -r)/build/

PWD=$(shell pwd)

all:
		make -C $(KDIR) M=$(PWD) modules
		gcc -o tasklet user_tasklet.c -lpthread
		
install:
		make -C $(KDIR) M=$(PWD) modules_install

clean:
		make -C $(KDIR) M=$(PWD) clean