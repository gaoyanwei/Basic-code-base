DIR_BIN_LIST:=$(DIR_BIN)/list
obj-m+=list.o
all:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	@test -d $(DIR_BIN_LIST) || mkdir $(DIR_BIN_LIST)
	@test -f list.ko && cp hlist.ko $(DIR_BIN_LIST)
clean:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
