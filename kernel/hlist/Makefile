DIR_BIN_HLIST:=$(DIR_BIN)/hlist
obj-m+=hlist.o
all:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	@test -d $(DIR_BIN_HLIST) || mkdir $(DIR_BIN_HLIST)
	@test -f hlist.ko && cp hlist.ko $(DIR_BIN_HLIST)
clean:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
