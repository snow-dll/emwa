CC=clang

emwa: emwa.c
	$(CC) -Wall emwa.c -lm -lz -Wextra -pedantic -std=gnu99 -o emwa

