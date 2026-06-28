obj-m := nanochrono_pmu.o

KDIR  ?= /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)

# Extra CFLAGS to include the local header.
ccflags-y += -I$(PWD)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

install: all
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install
	depmod -A

.PHONY: all clean install
