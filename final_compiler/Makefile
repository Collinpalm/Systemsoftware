# to run in terminal $ make
CC = gcc

# List all your .c (or .cpp or .cc) files here (source files, excluding header files)
SRC_CODE = driver.c lex.c parser.c vm.c

#################################

# default rule

all: $(SRC_CODE)
	$(CC) $(SRC_CODE) -lm
	