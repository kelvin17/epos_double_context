SUB_DIRS = 	apiext chain corebarrier coremsg coremutex corerwlock coresem coretod \
			heap interr isr object pheap \
			thread threadq timespec watchdog wkspace \
			userext

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