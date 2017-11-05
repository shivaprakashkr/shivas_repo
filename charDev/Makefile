obj-m+=chardev.o
 
all:
#	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
#	$(CC) chardev.c -o aatgc -I/home/shiva/excercise/linux/linux-4.10.1
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm aatgc
