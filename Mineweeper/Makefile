PREFIX  ?= /usr/local/bin
PROGRAM := ms
CFLAGS  ?= -Wall -Wextra -Wwrite-strings -pedantic -Ofast -march=native -s

$(PROGRAM): ms.c
	@gcc $(CFLAGS) -o $(PROGRAM) ms.c

install: $(PROGRAM)
	@install -s $(PROGRAM) $(PREFIX)/

uninstall:
	@rm -f $(PREFIX)/$(PROGRAM)
