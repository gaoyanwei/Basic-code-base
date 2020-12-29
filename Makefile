#变量定义
CC=gcc
CFLAGS=-wall -g -O2
DIR_TOP:=$(shell pwd)
DIR_BIN:=$(DIR_TOP)/bin
DIR_KERNEL:=$(DIR_TOP)/kernel
DIR_SRC:=$(DIR_TOP)/src
DIR_TEST:=$(DIR_TOP)/test

export DIR_TOP DIR_SRC_OBJS DIR_BIN DIR_TEST_OBJS CC CFLAGS

all:
	@echo "		name:		gaoyanwei Makefile"
	@echo "		help:		make help"
	@echo "		version:	1.0.0"

help:
	@echo "		build kernel: make kernel build"
	@echo "		clean kernel: make kernel clean"
	@echo "		build src	: make src build"
	@echo "		clean src	: make src clean"
	@echo "		build test	: make test build"
	@echo "		clean test	: make test clean"

kernel:
	@echo "make kernel waiting..."
	@mkdir $(DIR_BIN)
	@cd $(DIR_KERNEL)									\
		make Makefile all
	@echo "make kernel end"

src:
	@echo "make src waiting..."
	@mkdir $(DIR_BIN)
	@cd $(DIR_SRC)
#	@make Makefile
	@pwd
	@echo "make src end"

test:
	@echo "make test waiting..."
	@mkdir $(DIR_BIN)

clean:
	@rm -rf $(DIR_BIN)

.PHONY: kernel test src

.PHONY: clean ALL
