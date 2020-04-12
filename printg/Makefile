
all: a.out

a.out: main.c printg.c printg.h
	@gcc -Wall -Wextra -o a.out main.c printg.c

run: a.out
	@./a.out

test: test.c printg.c printg.h
	@gcc -Wall -Wextra -o test.out test.c printg.c
	@./test.out

clean:
	@rm -f a.out    && echo "[ OK ]  rm -f a.out"
	@rm -f test.out && echo "[ OK ]  rm -f test.out"
	@echo "Cleanup complete!"

.PHONY: all run test clean
