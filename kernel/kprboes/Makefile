DIR_BIN_KPROBES:=$(DIR_BIN)/kprobes
obj-m+=kprobes.o
all:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	@test -d $(DIR_BIN_KPROBES) || mkdir $(DIR_BIN_KPROBES)
	@test -f kprobes.ko && cp kprobes.ko $(DIR_BIN_KPROBES)
clean:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
