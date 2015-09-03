PROGRAM	=
INCLUDE = ./include
SRC	= ./src
TEST	= ./test
BIN	= ./bin
OBJ	= ./bin

CC	= gcc
CFLAGS	= -g -I $(INCLUDE)

# define DEBUG to show more logs
ifdef DEBUG
  CFLAGS += -DDEBUG
endif
LEX	= flex -i -I

# targets

all:
	@echo "Type 'make test' to test lexer."

.PHONY: test
test: $(BIN)/test_lex
	@echo "Running test_lex..."
	@bin/test_lex test/examples/expr.in
$(BIN)/test_lex: $(OBJ)/test_lex.o $(OBJ)/expr.yy.o -lfl

clean:
	-rm -f bin/*

# rules

$(SRC)/%.yy.c: $(SRC)/%.l
	$(LEX) -o $@ $<

$(BIN)/*.o: $(INCLUDE)/common.h $(INCLUDE)/helpers.h

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ)/%.o: $(TEST)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

