main: main.c
	gcc -o main main.c -Wall -Wextra -pedantic -std=c11

run: main
	./main
