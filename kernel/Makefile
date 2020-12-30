#定义变量
DIR_IOCTL:=$(DIR_KERNEL)/ioctl
DIR_FTRACE:=$(DIR_KERNEL)/ftrace
DIR_PROC:=$(DIR_KERNEL)/proc

help:
	@echo "****************************"
	@echo "* kernel help description:"
	@echo "* make ioctl"
	@echo "* make ftrace"
	@echo "* make proc"
	@echo "****************************"

ioctl:
	@echo "build ioctl driver start..."
	@cd $(DIR_IOCTL)	&& make		2>&1	> $(FILE_LOG)
	@echo "build ioctl driver end..."

ftrace:
	@echo "build ftrace driver start..."
	@cd $(DIR_FTRACE)	&& make		2>&1	> $(FILE_LOG)
	@echo "build ftrace driver end..."

proc:
	@echo "build proc driver start..."
	@cd $(DIR_PROC) && make		2>&1	> $(FILE_LOG)
	@echo "build proc driver end..."

clean:
	@echo "kernel clean start ..."
	@cd $(DIR_IOCTL)	&& make clean
	@cd $(DIR_FTRACE)	&& make clean
	@cd $(DIR_PROC)	&& make clean
	@echo "kernel clean end..."

.PHONY:ioctl ftrace clean proc
