DIR_BIN_FTRACE:=$(DIR_BIN)/ftrace
obj-m+=ftrace.o
all:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	@test -d $(DIR_BIN_FTRACE) || mkdir $(DIR_BIN_FTRACE)
	@test -f ftrace.ko && cp ftrace.ko $(DIR_BIN_FTRACE)
clean:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
