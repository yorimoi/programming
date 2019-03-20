exe=doraque
header=battle.c

$(exe): main.c battle.c
	@gcc -Wall -Wextra -o $(exe) main.c $(header)

run: $(exe)
	@./$(exe)
