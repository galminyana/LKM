obj-m += mymodule.o

modules:
	@$(MAKE) -C $(KERNEL_ROOT) M=$(shell pwd) modules
clean:
	@$(MAKE) -C $(KERNEL_ROOT) M=$(shell pwd) clean 
