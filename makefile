SRC_ROOT= $(shell chdir)
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
CFLAGS  = -c -O0 -g -w -I $(INCLUDE) -D__epos__ -D__rtems__ -U__GNUC__ -URTEMS_MULTIPROCESSING -D_OCC_NO_INLINE_
MAKE    = make

export LNK CC AR CFLAGS MAKE INCLUDE SRC_ROOT ALL_LIBS LIBS LIB_DIV LIB_C LIB_M

LIB_DIRS     = div newlib
MAIN_DIRS    = libbsp epos eposcore sapi libmisc libfs libcsupport 
MAIN_MODULES = $(patsubst %,%/thismodule.a,$(MAIN_DIRS))

SOURCES  = $(wildcard *.c)
ASM      = $(wildcard *.asm)
OBJS     = $(patsubst %.c,%.o,$(SOURCES))
ASM_OBJS = $(patsubst %.asm,%.o,$(ASM))

all: libs $(OS)
	$(MAKE) -C ECSwkspace
	
test:
	$(MAKE) -C testsuites

$(OS): noinline $(MAIN_DIRS) $(OBJS) $(ASM_OBJS) 
	$(AR) -r $(OS) $(ALL_OBJECTS)
	$(AR) -d $(OS) boot.o
	copy boot.o $(BOOT)
	
$(OBJS): %.o: %.c 
	$(CC) $(CFLAGS) $< -o $@ 
$(ASM_OBJS): %.o: %.asm
	$(CC) $(CFLAGS) $< -o $@  

.PHONY: $(MAIN_DIRS)
$(MAIN_DIRS):
	$(MAKE) -C $@
	$(AR) -x $@/thismodule.a

libs: $(LIB_DIRS)
.PHONY: $(LIB_DIRS)
$(LIB_DIRS):
	$(MAKE) -C $@

os: $(OS)
	$(MAKE) -C ECSwkspace

ALL_OBJECTS=$(wildcard *.o)	

.PHONY: clean 
clean:
	del /F/S *.o
	del /F/S mylib.a
	del /F/S boot.o
	del /F/S div.a
	del /F/S newlib_libc.a
	del /F/S newlib_libm.a
	del	/F/S libc.a
	del	/F/S libm.a
	del /F/S thismodule.a
	del /F *.out
	del /F/S *~

.PHONY: pclean 
pclean:
	del /F/S mylib.a
	del /F/S boot.o
	del /F/S div.a
	del /F/S newlib_libc.a
	del /F/S newlib_libm.a
	del	/F/S libc.a
	del	/F/S libm.a
	del /F/S thismodule.a
	del /F *.out
	del /F/S *~
	
.PHONY: osclean 
osclean:
	del /F/S *.o
	del /F/S mylib.a
	del /F/S boot.o	
	del /F/S thismodule.a
	del /F *.out
	del /F/S *~
	del /F/S *_inl.c
	
noinline:
	copy $(INCLUDE)\rtems\chain.inl						$(SRC_ROOT)\libmisc\chain_inl.c
	copy $(INCLUDE)\rtems\extension.inl				$(SRC_ROOT)\libmisc\extension_inl.c	
	copy $(INCLUDE)\rtems\rtems\asr.inl				$(SRC_ROOT)\libmisc\asr_inl.c
	copy $(INCLUDE)\rtems\rtems\attr.inl			$(SRC_ROOT)\libmisc\attr_inl.c
	copy $(INCLUDE)\rtems\rtems\barrier.inl		$(SRC_ROOT)\libmisc\barrier_inl.c
	copy $(INCLUDE)\rtems\rtems\event.inl			$(SRC_ROOT)\libmisc\event_inl.c
	copy $(INCLUDE)\rtems\rtems\eventset.inl	$(SRC_ROOT)\libmisc\eventset_inl.c
	copy $(INCLUDE)\rtems\rtems\message.inl		$(SRC_ROOT)\libmisc\message_inl.c
	copy $(INCLUDE)\rtems\rtems\modes.inl			$(SRC_ROOT)\libmisc\modes_inl.c
	copy $(INCLUDE)\rtems\rtems\options.inl		$(SRC_ROOT)\libmisc\options_inl.c
	copy $(INCLUDE)\rtems\rtems\part.inl			$(SRC_ROOT)\libmisc\part_inl.c
	copy $(INCLUDE)\rtems\rtems\ratemon.inl		$(SRC_ROOT)\libmisc\ratemon_inl.c
	copy $(INCLUDE)\rtems\rtems\region.inl		$(SRC_ROOT)\libmisc\region_inl.c
	copy $(INCLUDE)\rtems\rtems\sem.inl				$(SRC_ROOT)\libmisc\sem_inl.c
	copy $(INCLUDE)\rtems\rtems\status.inl		$(SRC_ROOT)\libmisc\status_inl.c
	copy $(INCLUDE)\rtems\rtems\support.inl		$(SRC_ROOT)\libmisc\support_inl.c
	copy $(INCLUDE)\rtems\rtems\tasks.inl			$(SRC_ROOT)\libmisc\tasks_inl.c
	copy $(INCLUDE)\rtems\rtems\timer.inl			$(SRC_ROOT)\libmisc\timer_inl.c
	copy $(INCLUDE)\rtems\score\address.inl				$(SRC_ROOT)\libmisc\address_inl.c
	copy $(INCLUDE)\rtems\score\chain.inl					$(SRC_ROOT)\libmisc\corechain_inl.c
	copy $(INCLUDE)\rtems\score\corebarrier.inl		$(SRC_ROOT)\libmisc\corebarrier_inl.c
	copy $(INCLUDE)\rtems\score\coremsg.inl				$(SRC_ROOT)\libmisc\coremsg_inl.c
	copy $(INCLUDE)\rtems\score\coremutex.inl			$(SRC_ROOT)\libmisc\coremutex_inl.c
	copy $(INCLUDE)\rtems\score\corerwlock.inl		$(SRC_ROOT)\libmisc\corerwlock_inl.c
	copy $(INCLUDE)\rtems\score\coresem.inl				$(SRC_ROOT)\libmisc\coresem_inl.c
	copy $(INCLUDE)\rtems\score\corespinlock.inl	$(SRC_ROOT)\libmisc\corespinlock_inl.c
	copy $(INCLUDE)\rtems\score\heap.inl					$(SRC_ROOT)\libmisc\heap_inl.c
	copy $(INCLUDE)\rtems\score\isr.inl						$(SRC_ROOT)\libmisc\isr_inl.c
	copy $(INCLUDE)\rtems\score\mppkt.inl					$(SRC_ROOT)\libmisc\mppkt_inl.c
	copy $(INCLUDE)\rtems\score\object.inl				$(SRC_ROOT)\libmisc\object_inl.c
	copy $(INCLUDE)\rtems\score\objectmp.inl			$(SRC_ROOT)\libmisc\objectmp_inl.c
	copy $(INCLUDE)\rtems\score\priority.inl			$(SRC_ROOT)\libmisc\priority_inl.c
	copy $(INCLUDE)\rtems\score\protectedheap.inl	$(SRC_ROOT)\libmisc\protectedheap_inl.c
	copy $(INCLUDE)\rtems\score\stack.inl					$(SRC_ROOT)\libmisc\stack_inl.c
	copy $(INCLUDE)\rtems\score\states.inl				$(SRC_ROOT)\libmisc\states_inl.c
	copy $(INCLUDE)\rtems\score\sysstate.inl			$(SRC_ROOT)\libmisc\sysstate_inl.c
	copy $(INCLUDE)\rtems\score\thread.inl				$(SRC_ROOT)\libmisc\thread_inl.c
	copy $(INCLUDE)\rtems\score\threadmp.inl			$(SRC_ROOT)\libmisc\threadmp_inl.c
	copy $(INCLUDE)\rtems\score\threadq.inl				$(SRC_ROOT)\libmisc\threadq_inl.c
	copy $(INCLUDE)\rtems\score\tod.inl						$(SRC_ROOT)\libmisc\tod_inl.c
	copy $(INCLUDE)\rtems\score\tqdata.inl				$(SRC_ROOT)\libmisc\tqdata_inl.c
	copy $(INCLUDE)\rtems\score\userext.inl				$(SRC_ROOT)\libmisc\userext_inl.c
	copy $(INCLUDE)\rtems\score\watchdog.inl			$(SRC_ROOT)\libmisc\watchdog_inl.c
	copy $(INCLUDE)\rtems\score\wkspace.inl				$(SRC_ROOT)\libmisc\wkspace_inl.c
