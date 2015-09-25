PROGRAM	=
INCLUDE = ./include
SRC	= ./src
TEST	= ./test
BIN	= ./bin
OBJ	= ./bin

CC	= clang++
CFLAGS	= -g -I $(INCLUDE) -I $(SRC)
CPPFLAGS= $(CFLAGS) -std=c++11

# define DEBUG to show more logs
ifndef RELEASE
  CFLAGS += -DDEBUG
endif
LEX	= flex -I
YACC	= bison

# targets

all:
	@echo "Type 'make test' to test lexer."

.PHONY: test
test: test_parser

.PHONY: test_parser
test_parser: $(BIN)/test_parser

$(BIN)/kaleidoscope: $(OBJ)/kaleidoscope.o

$(BIN)/test_parser: $(OBJ)/Parser.tab.o $(OBJ)/Lexer.yy.o $(OBJ)/test_parser.o

$(OBJ)/test_parser.o: $(SRC)/Parser.tab.cc

.PHONY: clean
clean:
	-rm -f $(BIN)/*
	-rm -f $(SRC)/*.cc $(SRC)/*.hh

# targets for TA

# rules

$(SRC)/%.yy.cc: $(SRC)/%.ll
	$(LEX) -o $@ $<

$(SRC)/%.tab.cc: $(SRC)/%.yy
	$(YACC) -o $@ $< 

$(BIN)/*.o: $(INCLUDE)/helpers.h

$(OBJ)/%.o: $(SRC)/%.cc
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CPPFLAGS) -Wall -c -o $@ $<

$(OBJ)/%.o: $(TEST)/%.cpp
	$(CC) $(CPPFLAGS) -Wall -c -o $@ $<
