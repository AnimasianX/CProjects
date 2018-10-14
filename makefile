CFLAGS = -g -Wall
TARGET = ex17

all: $(TARGET)
$(TARGET): $(TARGET).c
	cc $(CFLAGS) -o $(TARGET) $(TARGET).c

clean: 
	$(RM) $(TARGET)
