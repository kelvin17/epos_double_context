SUB_DIRS = libm libc

all: $(SUB_DIRS)
.PHONY: $(SUB_DIRS)
$(SUB_DIRS):
	$(MAKE) -C $@

.PHONY: clean
clean:
	del /S/F *.a *.o *.log