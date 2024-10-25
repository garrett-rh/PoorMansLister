default: build

build:
	gcc -std=c17 -Wpedantic -pedantic-errors -Werror -Wall main.c -o lister
