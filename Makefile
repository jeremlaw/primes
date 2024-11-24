# Makefile for Prime Number Generator
#
# Created November 23, 2024

############## Variables ###############

CC = gcc -O3 # The compiler being used
CFLAGS = -g # Compiler flags
RM = rm -f # Command to remove files

TARGET = run # Target executable
OBJS = run.o generator.o dynamic_array.o # Object files needed for target

############### Rules ###############

all: run

## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


## Linking step (.o -> executable program)

$(TARGET): $(OBJS) 
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(TARGET) $(OBJS)