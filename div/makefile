SOURCES=$(wildcard *.c)
ASM = $(wildcard *.s)
TARGET = div.a
OBJS = $(patsubst %.c,%.o,$(SOURCES))
ASM_OBJS = $(patsubst %.s,%.o,$(ASM))

$(TARGET): $(OBJS) $(ASM_OBJS)
	$(AR) -r $(TARGET) $(OBJS) $(ASM_OBJS)
	copy $(TARGET) $(LIB_DIV)
	
$(OBJS): %.o: %.c 
	$(CC) $(CFLAGS) $< -o $@ 
$(ASM_OBJS): %.o: %.s
	$(CC) $(CFLAGS) $< -o $@ 
	
OBJECTS=$(wildcard *.o)	

.PHONY: clean
clean:
	del /F *.o *.a *.log
