SUB_DIRS = argz ctype errno locale misc posix reent search signal stdio stdlib string syscalls time 
TARGET = libc.a
SUB_MODULES_OBJECTS = $(wildcard *.o)
CFLAGS += -DMALLOC_PROVIDED
export CFLAGS

$(TARGET): $(SUB_DIRS) $(SUB_MODULES_OBJECTS)
	$(AR) -r $(TARGET) $(SUB_MODULES_OBJECTS)
	copy $(TARGET) $(LIB_C)

.PHONY: $(SUB_DIRS)
$(SUB_DIRS):
	$(MAKE) -C $@
	$(AR) -x $@/thismodule.a

.PHONY: clean
clean:
	del /S/F *.a *.o *.log