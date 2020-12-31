DIR_BIN_HOOK_SYSCALL_TABLE:=$(DIR_BIN)/hook_syscall_table
obj-m+=hook_syscall_table.o
all:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	@test -d $(DIR_BIN_HOOK_SYSCALL_TABLE) || mkdir $(DIR_BIN_HOOK_SYSCALL_TABLE)
	@test -f hook_syscall_table.ko && cp hook_syscall_table.ko $(DIR_BIN_HOOK_SYSCALL_TABLE)
clean:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
