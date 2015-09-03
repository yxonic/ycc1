PROGRAM	=
INCLUDE = ./include
SRC	= ./src
TEST	= ./test
BIN	= ./bin
OBJ	= ./bin

CC	= gcc
CFLAGS	= -m32 -g
INC	= -I $(INCLUDE)
# define DEBUG to show more logs
ifdef DEBUG
  CFLAGS += -DDEBUG
endif
LEX	= flex -i -I

.PHONY: test

all:
	@echo "Type 'make test' to test lexer."

test: $(BIN)/test_lex
	@echo "Running test_lex..."
	@bin/test_lex test/examples/expr.in

$(SRC)/expr.yy.c: $(SRC)/expr.lex
	$(LEX) -o $(SRC)/expr.yy.c $(SRC)/expr.lex

$(BIN)/test_lex: $(TEST)/test_lex.c $(SRC)/expr.yy.c
	$(CC) $(CFLAGS) $(INC) $(TEST)/test_lex.c $(SRC)/expr.yy.c -o $(BIN)/test_lex -lfl
