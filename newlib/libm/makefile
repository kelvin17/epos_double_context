SUB_DIRS = common math
TARGET = libm.a
SUB_MODULES_OBJECTS = $(wildcard *.o)

$(TARGET): $(SUB_DIRS) $(SUB_MODULES_OBJECTS)
	$(AR) -r $(TARGET) $(SUB_MODULES_OBJECTS)
	copy $(TARGET) $(LIB_M)

.PHONY: $(SUB_DIRS)
$(SUB_DIRS):
	$(MAKE) -C $@
	$(AR) -x $@/thismodule.a

.PHONY: clean
clean:
	del /S/F *.a *.o *.log