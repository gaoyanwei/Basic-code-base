DIR_BIN_IOCTL:=$(DIR_BIN)/ioctl
FILE_TEST:=ioctl_test
obj-m+=ioctl.o
all:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	@test -d $(DIR_BIN_IOCTL) || mkdir $(DIR_BIN_IOCTL)
	@test -f ioctl.ko && cp ioctl.ko $(DIR_BIN_IOCTL)
	@$(CC) $(CFLAGS) ioctl_test.c -o $(FILE_TEST)
	@test -f $(FILE_TEST) && cp $(FILE_TEST) $(DIR_BIN_IOCTL)
clean:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
