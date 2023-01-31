CONFIG_MODULE_SIG=n
CONFIG_MODULE_SIG_ALL=n

obj-m := characterdriver.o
KERNELDIR ?= /lib/modules/$(shell uname -r)/build 
PWD := $(shell pwd)
all:
	@echo make files...
	make -C $(KERNELDIR) M=$(PWD) modules
	@gcc -g -Wall -O2 test.c -o test
.PHONY: clean
clean:
	@echo remove files...
	make -C $(KERNELDIR) M=$(PWD) clean
	@rm -f test
