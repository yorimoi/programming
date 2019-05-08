TARGET ?= mybf
CPPGLAGS ?= -Wall -Wextra -pedantic-errors -std=c++11
CFLAGS ?= -Wall -Wextra -pedantic-errors

$(TARGET): main.c
	@gcc $(CGLAGS) -o $(TARGET) main.c

run: $(TARGET)
	@./$(TARGET)

test: $(TARGET) test.sh
	@./test.sh

clean:
	$(RM) $(TARGET)
