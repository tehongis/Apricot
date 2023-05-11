run			: main.exe
	./main.exe
main.exe	: main.c
	gcc -o main.exe -Wall main.c
