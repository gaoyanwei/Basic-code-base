#定义变量
DIR_IOCTL:=$(DIR_KERNEL)/ioctl
DIR_FTRACE:=$(DIR_KERNEL)/ftrace
DIR_PROC:=$(DIR_KERNEL)/proc
DIR_KPRBOES:=$(DIR_KERNEL)/kprboes
DIR_RBTREE:=$(DIR_KERNEL)/rbtree
DIR_HLIST:=$(DIR_KERNEL)/hlist
DIR_HOOK_SYSCALL_TABLE:=$(DIR_KERNEL)/hook_syscall_table

help:
	@echo "****************************"
	@echo "* kernel help description:"
	@echo "* make ioctl"
	@echo "* make ftrace"
	@echo "* make proc"
	@echo "* make kprboes"
	@echo "* make rbtree"
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

kprboes:
	@echo "build proc driver start..."
	@cd $(DIR_KPRBOES) && make		2>&1	> $(FILE_LOG)
	@echo "build proc driver end..."

rbtree:
	@echo "build rbtree driver start..."
	@cd $(DIR_RBTREE) && make		2>&1	> $(FILE_LOG)
	@echo "build rbtree driver end..."

hlist:
	@echo "build hlist driver start..."
	@cd $(DIR_HLIST) && make		2>&1	> $(FILE_LOG)
	@echo "build hlist driver end..."

hook_syscall_table:
	@echo "build hlist driver start..."
	@cd $(DIR_HOOK_SYSCALL_TABLE) && make		2>&1	> $(FILE_LOG)
	@echo "build hlist driver end..."

clean:
	@echo "kernel clean start ..."
	@cd $(DIR_IOCTL)	&& make clean
	@cd $(DIR_FTRACE)	&& make clean
	@cd $(DIR_PROC)	&& make clean
	@cd $(DIR_KPRBOES)	&& make clean
	@cd $(DIR_RBTREE)	&& make clean
	@cd $(DIR_HLIST)	&& make clean
	@cd $(DIR_HOOK_SYSCALL_TABLE)	&& make clean
	@echo "kernel clean end..."

.PHONY:ioctl ftrace clean proc kprboes rbtree hlist hook_syscall_table
