obj-m := work_queue.o
KERNELDIR ?= /lib/modules/$(shell uname -r)/build 
PWD := $(shell pwd)
all:
	@echo make files...
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	@echo remove files...
	make -C $(KERNELDIR) M=$(PWD) clean

