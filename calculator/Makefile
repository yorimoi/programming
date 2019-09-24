CC := /mnt/c/Windows/Microsoft.NET/Framework64/v4.0.30319/csc.exe
CSFLAGS = /unsafe
TARGET := cal.exe

all: $(TARGET)

$(TARGET): cal.cs
	$(CC) $(CSFLAGS) -out:$@ $^

test: $(TARGET)
	./test.sh

clean:
	rm -rf *.exe

.PHONY: all clean test
