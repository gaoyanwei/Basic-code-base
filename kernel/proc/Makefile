DIR_BIN_PROC:=$(DIR_BIN)/proc
obj-m+=proc.o
all:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	@test -d $(DIR_BIN_PROC) || mkdir $(DIR_BIN_PROC)
	@test -f proc.ko && cp proc.ko $(DIR_BIN_PROC)
clean:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
