# Directpries
MODULES = ../../modules
INCLUDE = ../../include

# Compiler
CC = gcc

# Copiler options
CFLAGS = -g -ggdb3

# Object files
OBJS_FILES = main.o Commands.o pathfinder.o utilities.o minimax.o

# Executable name
EXEC = main.exe

minmax: $(OBJS_FILES)
	$(CC) $(OBJS_FILES) -o $(EXEC)
	rm -f $(OBJS_FILES)
