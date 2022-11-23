obj-$(CONFIG_IRQMAP) += irqmap.o

all:
	$(MAKE) ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C ../../../linux-4.9 M=$(PWD) modules
