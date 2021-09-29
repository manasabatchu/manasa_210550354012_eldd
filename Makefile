obj-m +=driver.o
KDIR=/lib/modules/$(shell uname -r)/build/
PWD=$(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
	gcc -o read read.c
	gcc -o write write.c


clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -rf read write
