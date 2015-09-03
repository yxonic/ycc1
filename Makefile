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
	@mkdir -p $(BIN)
	@echo "Running test_lex..."
	@$(BIN)/test_lex $(TEST)/examples/expr.in
$(BIN)/test_lex: $(OBJ)/test_lex.o $(OBJ)/expr.yy.o -lfl

clean:
	-rm -f bin/*

# rules

$(SRC)/%.yy.c: $(SRC)/%.l
	$(LEX) -o $@ $<

$(BIN)/*.o: $(INCLUDE)/common.h $(INCLUDE)/helpers.h

$(OBJ)/%.yy.o: $(SRC)/%.yy.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -Wall -c -o $@ $<

$(OBJ)/%.o: $(TEST)/%.c
	$(CC) $(CFLAGS) -Wall -c -o $@ $<

