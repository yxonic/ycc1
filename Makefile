all: pre-build
	@echo 'Building with CMake...'
	@cd dist; cmake ../; make -s
	@echo 'Now you can run "make test" to run tests.'

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
P2: test_scanner

P3: test_kaleidoscope
	@echo
	@echo '* Check doc/P3.md for answers.'
