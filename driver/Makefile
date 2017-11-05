obj-m+=keydri.o

INC=-I/home/shiva/excercise/linux/linux-4.10.1/include
KDIR=/lib/modules/${shell uname -r}/build
#KDIR=/home/shiva/excercise/linux/linux-4.10.1

all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules
clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
