default: build

build:
	gcc -std=c17 -Wpedantic -pedantic-errors -Werror -Wall -Wextra main.c -o lister