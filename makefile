main.exe: main.o Commands.o genmove.o
	gcc -o main.exe main.o Commands.o genmove.o

# make object files
main.o: main.c
	gcc -c main.c

Commands.o: Commands.c
	gcc -c Commands.o

genmove.o: genmove.c
	gcc -c genmove.c

run:
	./main.exe