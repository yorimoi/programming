run: main.c stack.c token.c token.h ast.c ast.h
	@$(CC) main.c stack.c token.c ast.c
	@./a.out

test: test.c main.c stack.c token.c token.h ast.c ast.h
	@$(CC) test.c main.c stack.c token.c ast.c -Wl,--entry=test
	@./a.out

clean:
	rm -f a.out

.PHONY:
	run test clean
