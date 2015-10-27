export CC=clang-3.6
export CXX=clang++-3.6

all: pre-build
	@echo 'Building with CMake...'
	@cd dist; cmake ../; make -s
	@echo 'Now you can run "make test" to run tests.'

.PHONY: pre-build
pre-build:
	@mkdir -p dist

.PHONY: test doc clean
test: test_parser test_kaleidoscope

doc:
	@echo 'Generating document using doxygen...'
	@cd Doxygen; doxygen 2>/dev/null
	@echo 'Finished. Check Doxygen/html/index.html'

clean:
	-rm -rf dist
	-rm -f *.png

test_kaleidoscope: pre-build
	@cd dist; cmake ../; make -s kaleidoscope
	@echo
	@tput setaf 3; echo '* Test input files are in test/examples/. Output files are test*.png.'; tput sgr0
	dist/kaleidoscope < dist/examples/kaleidoscope_ex1.k 2>/dev/null
	dot toy.dot -Tpng -o test1.png
	dist/kaleidoscope < dist/examples/kaleidoscope_ex2.k 2>/dev/null
	dot toy.dot -Tpng -o test2.png
	-rm toy.dot

test_parser: pre-build
	@cd dist; cmake ../; make -s test_parser
	@echo
	@tput setaf 3; echo '* Run dist/test_parser'; tput sgr0
	dist/test_parser -d dist/examples/example0.c1
	@echo
	dist/test_parser -d dist/examples/example1.c1
	@echo
	dist/test_parser -d dist/examples/example2.c1
	@echo
	dist/test_parser -d dist/examples/example3.c1
	@echo
	dist/test_parser dist/examples/example4.c1

test_utils: pre-build
	@cd dist; cmake ../; make -s test_utils
	@echo
	dist/test_utils
	@tput setaf 3; echo '* Some logged to test.log'; tput sgr0
	cat test.log
	rm -f test.log


# Targets for TA
P1:
	@echo 'Please checkout to tag P1 to run tests for P1.'

P2:
	@echo 'Please checkout to tag P2 to run tests for P2.'

P3: test_kaleidoscope
	@echo
	@tput setaf 3; echo '* Check doc/P3.md for answers.'; tput sgr0

P4: test_parser
	@echo
	@tput setaf 3; echo '* Check doc/P4.md for answers.'; tput sgr0

