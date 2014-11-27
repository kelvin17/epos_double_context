#SRC_ROOT= F:\rtems\20140903epos\epos_master_xlanchen_rearch
INCLUDE = $(SRC_ROOT)\include
LIBS    = $(SRC_ROOT)\libs
LIB_DIV = $(SRC_ROOT)\libs\div.a
LIB_C   = $(SRC_ROOT)\libs\newlib_libc.a
LIB_M   = $(SRC_ROOT)\libs\newlib_libm.a
OS      = $(SRC_ROOT)\libs\mylib.a
BOOT    = $(SRC_ROOT)\libs\boot.o
ALL_LIBS= $(OS) $(LIB_DIV) $(LIB_C) $(LIB_M) 
LNK     = eld
CC      = occ
AR      = ar
CFLAGS  = -c -O0 -g -I $(INCLUDE) -D__epos__ -D__rtems__ -U__GNUC__ -URTEMS_MULTIPROCESSING
MAKE    = make

all: clean bwcompile 

bwcompile: 
	$(CC) $(CFLAGS)  init.c -o init.o 
	$(LNK) $(BOOT) init.o -( $(ALL_LIBS) -) -o semaphore.out -T sample1.cmd -Map xxxmap --cref --no-whole-archive
	objdump -t semaphore.out > log
	ldasm -d semaphore.out

clean: 
	rm -f *.s 
	rm -f *.sl 
	rm -f *.o 
	rm -f *.out 
	rm -f *.dis 
