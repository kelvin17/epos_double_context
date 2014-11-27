SOURCES=$(wildcard *.c)
ASM = $(wildcard *.asm)

OBJS = $(patsubst %.c,%.o,$(SOURCES))
ASM_OBJS = $(patsubst %.asm,%.o,$(ASM))

thismodule.a: $(OBJS) $(ASM_OBJS)
	$(AR) -r thismodule.a $(OBJS) $(ASM_OBJS)
	
$(OBJS): %.o: %.c 
	$(CC) $(CFLAGS) $< -o $@ 
$(ASM_OBJS): %.o: %.asm
	$(CC) $(CFLAGS) $< -o $@ 
	
OBJECTS=$(wildcard *.o)	

.PHONY: clean
clean:
	del /F *.o *.a *.log
