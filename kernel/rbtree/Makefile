DIR_BIN_RBTREE:=$(DIR_BIN)/rbtree
obj-m+=rbtree.o
all:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	@test -d $(DIR_BIN_RBTREE) || mkdir $(DIR_BIN_RBTREE)
	@test -f rbtree.ko && cp rbtree.ko $(DIR_BIN_RBTREE)
clean:
	@make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
