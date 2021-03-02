build: main.h main.c
	@$(CC) main.c

run: main.h main.c
	@$(CC) main.c
	@./a.out

test: main.h main.c test.c
	@$(CC) test.c main.c -Wl,--entry=test
	@./a.out

clean:
	rm -f a.out *.o *~ tmp*

.PHONY: build run test clean
