#变量定义
DIR_TOP:=$(shell pwd)
DIR_BIN:=$(DIR_TOP)/bin
DIR_KERNEL:=$(DIR_TOP)/kernel
DIR_SRC:=$(DIR_TOP)/src
DIR_TEST:=$(DIR_TOP)/test
FILE_LOG:=$(DIR_TOP)/Build_log.txt

MAKE=make
OPTIONS=help
CC=gcc
CFLAGS=-g -Wall -O2
DIR_INC=$(DIR_TOP)/inc

export DIR_TOP DIR_SRC DIR_BIN DIR_TEST DIR_KERNEL CC MAKE CFLAGS FILE_LOG DIR_INC

all:
	@echo "		name:		gaoyanwei Makefile"
	@echo "		help:		make help"
	@echo "		version:	1.0.0"

#OPTIONS 是编译的选项 是下级目录的Makefile的编译选项
help:
	@echo ""
	@echo "	build kernel:	make kernel OPTIONS=DIR"
	@echo "	build src:	make src OPTIONS=DIR"
	@echo "	build test:	make test OPTIONS=DIR"
	@echo "	clean git:	make clean"
	@echo ""

kernel:
	@echo "make kernel waiting..."
	@test -d $(DIR_BIN) || mkdir $(DIR_BIN)
	@$(MAKE) -C $(DIR_KERNEL) $(OPTIONS)			2>&1	>$(FILE_LOG)
	@echo "make kernel end"

src:
	@echo "make src waiting..."
	@test -d $(DIR_BIN) || mkdir $(DIR_BIN)
	@$(MAKE) -C $(DIR_SRC) $(OPTIONS)				2>&1	> $(FILE_LOG)
	@echo "make src end"

test:
	@echo "make test waiting..."
	@test -d $(DIR_BIN) || mkdir $(DIR_BIN)
	@$(MAKE) -C $(DIR_TEST) $(OPTIONS)				2>&1	> $(FILE_LOG)
	@echo "make test end"

clean:
	@echo "clean git start..."
	@$(MAKE) -C $(DIR_KERNEL) clean					2>&1	> $(FILE_LOG)
	@rm -rf $(DIR_BIN) $(FILE_LOG)

.PHONY: kernel test src clean
