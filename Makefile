all: pre-build
	@echo 'Building with CMake...'
	@cd dist; cmake ../; make -s
	@echo 'Now you can run "make test" to run tests.'

<<<<<<< HEAD
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
=======
.PHONY: pre-build
pre-build:
	@mkdir -p dist

.PHONY: test clean
test: test_scanner test_kaleidoscope

clean:
	-rm -rf dist
	-rm -f *.png

.PHONY: test_scanner
test_scanner: pre-build
	@cd dist; cmake ../; make -s test_flex_lexer
	-dist/test_flex_lexer dist/examples/example1.c1
	-dist/test_flex_lexer dist/examples/example2.c1
	-dist/test_flex_lexer dist/examples/example3.c1

.PHONY: test_kaleidoscope
test_kaleidoscope: pre-build
	@cd dist; cmake ../; make -s kaleidoscope
	@echo
	@echo '* Test input files are in test/examples/. Output files are test*.png.'
	dist/kaleidoscope < dist/examples/kaleidoscope_ex1.k 2&>/dev/null
	dot toy.dot -Tpng -o test1.png
	dist/kaleidoscope < dist/examples/kaleidoscope_ex2.k 2&>/dev/null
	dot toy.dot -Tpng -o test2.png
	-rm toy.dot

# Targets for TA
P1:
	@echo 'Please checkout to tag P1 to run tests for P1.'

P2: test_scanner

P3: test_kaleidoscope
	@echo
	@echo '* Check doc/P3.md for answers.'
>>>>>>> master
