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
test: $(BIN)/test_expr $(BIN)/test_flex_lexer
	@mkdir -p $(BIN)
	@echo
	@echo "Testing expression lexer..."
	@echo
	@echo "Scanning $(TEST)/examples/expr.in"
	@$(BIN)/test_expr $(TEST)/examples/expr.in
	@echo
	@echo "Testing flex lexer..."
	@echo
	@echo "Scanning $(TEST)/examples/example1.c1"
	@$(BIN)/test_flex_lexer $(TEST)/examples/example1.c1

$(BIN)/test_expr: $(OBJ)/test_expr.o $(OBJ)/expr.yy.o -lfl

$(BIN)/test_flex_lexer: $(OBJ)/test_flex_lexer.o $(OBJ)/c1.yy.o -lfl

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

