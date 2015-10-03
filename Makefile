.PHONY: all test
all:
	@echo 'Building with CMake...'
	@mkdir -p dist
	@cd dist; cmake ../; make
	@echo 'Now you can run "make test" to run tests.'

test: test_scanner test_kaleidoscope

test_scanner:
	-dist/test_flex_lexer dist/examples/example1.c1
	-dist/test_flex_lexer dist/examples/example2.c1
	-dist/test_flex_lexer dist/examples/example3.c1

test_kaleidoscope:

# Targets for TA
P2: all test_scanner
