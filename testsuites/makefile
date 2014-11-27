SRC_ROOT= $(shell chdir)\..
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
CFLAGS  = -c -O0 -g  -I $(INCLUDE) -D__epos__ -D__rtems__ -U__GNUC__ -URTEMS_MULTIPROCESSING -D_OCC_NO_INLINE_
MAKE    = make

export LNK CC AR CFLAGS MAKE INCLUDE SRC_ROOT ALL_LIBS LIBS LIB_DIV LIB_C LIB_M

TESTDIRS = sp13 sp12
#sp02 sp03 sp04 sp05 sp06 sp08 sp11 sp12 sp13 sp14 sp15 sp16 sp17 sp19
	
test:$(TESTDIRS)

.PHONY:$(TESTDIRS)
$(TESTDIRS):main.o
	copy makefile_test $@\makefile
	$(MAKE) -C $@ test

main.o:
	$(CC) $(CFLAGS) main.c -o main.o
	
clean:
	del /F/S *.o	
	del /F/S *.out
	del /F/S *.dis
	del /F/S *.symbol
	del /F/S .cvsignore
	del /F/S Makefile.am
	del /F/S *.scn