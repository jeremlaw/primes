# Makefile for Prime Number Generator
#
# Created November 23, 2024

############## Variables ###############

CC = gcc -O2 # The compiler being used
CFLAGS = -g # Compiler flags
RM = rm -f # Command to remove files

TARGET1 = run # Target executable 1
OBJS1 = run.o generator.o dynamic_array.o bit_array.o # Object files needed for target 1
TARGET2 = time # Target executable 2
OBJS2 = time_data.o generator.o dynamic_array.o bit_array.o # Object files needed for target 2
TARGET3 = nthprime # Target executable 3
OBJS3 = nthprime.o # Object files needed for target 3

TARGETS = run time nthprime
OBJS = run.o time_data.o nthprime.o generator.o dynamic_array.o bit_array.o

############### Rules ###############

all: run time nthprime

## Compile step (.c files -> .o files)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


## Linking step (.o -> executable program)

$(TARGET1): $(OBJS1) 
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET2): $(OBJS2) 
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET3): $(OBJS3) 
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(TARGETs) $(OBJS)
