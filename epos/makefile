SUB_DIRS = 	attr barrier clock eposobject event intr msg \
			part ratemon region sem signal task \
			timer workspace 

SUB_MODULES_OBJECTS = $(wildcard *.o)

thismodule.a: $(SUB_DIRS) $(SUB_MODULES_OBJECTS)
	$(AR) -r thismodule.a $(SUB_MODULES_OBJECTS)

.PHONY: $(SUB_DIRS)
$(SUB_DIRS):
	$(MAKE) -C $@
	$(AR) -x $@/thismodule.a

.PHONY: clean
clean:
	del /S/F *.a *.o *.log