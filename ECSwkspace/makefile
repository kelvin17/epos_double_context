#SUB_DIRS = 	sample1 partition_test region_test task_switch_test semaphore_test semaphore_manager_test message_test clock_interrupt_test task_wake_test
SUB_DIRS = restart clock_interrupt_test
#SRC_ROOT= $(shell chdir)\..\
INCLUDE = $(SRC_ROOT)\include
LIBS    = $(SRC_ROOT)\libs
LIB_DIV = $(SRC_ROOT)\libs\div.a
LIB_C   = $(SRC_ROOT)\libs\newlib_libc.a
LIB_M   = $(SRC_ROOT)\libs\newlib_libm.a
OS      = $(SRC_ROOT)\libs\mylib.a
BOOT    = $(SRC_ROOT)\libs\boot.o
ALL_LIBS= $(LIB_DIV) $(LIB_C) $(LIB_M) $(OS)
LNK     = eld
CC      = occ
AR      = ar
CFLAGS  = -c -O0 -w -g -I $(INCLUDE) -D__epos__ -D__rtems__ -U__GNUC__ -URTEMS_MULTIPROCESSING
MAKE    = make

all:$(SUB_DIRS)

.PHONY: $(SUB_DIRS)
$(SUB_DIRS):
	$(MAKE) -C $@

.PHONY: clean
clean:
	del /S/F *.a *.o *.log
