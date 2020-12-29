#定义变量
DIR_IOCTL:=$(DIR_KERNEL)/ioctl

help:
	@echo "****************************"
	@echo "* kernel help description:"
	@echo "* make ioctl"
	@echo "* make ftrace"
	@echo "****************************"

ioctl:
	@echo "build ioctl driver start..."
	@cd $(DIR_IOCTL)	&& make		2>&1	> $(FILE_LOG)
	@echo "build ioctl driver end..."
ftrace:

clean:
	@echo "kernel clean start ..."
	@cd $(DIR_IOCTL)	&& make clean
	@echo "kernel clean end..."

.PHONY:ioctl ftrace clean
