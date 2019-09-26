CC := gcc
CFLAGS := -Wall -Wextra -pedantic
CSC := /mnt/c/Windows/Microsoft.NET/Framework64/v4.0.30319/csc.exe
CSFLAGS := /unsafe
TARGET := cal.exe
CTARGET := cal

all: $(TARGET)

$(TARGET): cal.cs
	$(CSC) $(CSFLAGS) -out:$@ $^

$(CTARGET): cal.c
	$(CC) $(CFLAGS) -o $@ $^

test: $(TARGET)
	./test.sh

testc: $(CTARGET)
	@mv $(CTARGET) $(TARGET)
	./test.sh
	@mv $(TARGET) $(CTARGET)

repl: $(CTARGET)
	@./$(CTARGET) repl

clean:
	rm -rf *.exe
	rm -rf $(CTARGET)

.PHONY: all clean test testc repl
