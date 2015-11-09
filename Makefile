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
	@mkdir -p Doxygen
	@cd Doxygen; doxygen ../Doxyfile
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
	@echo
	dist/test_parser dist/examples/example5.c1

test_dumpdot: pre-build
	@cd dist; cmake ../; make -s test_dumpdot
	@echo
	@tput setaf 3; echo '* Run dist/test_parser'; tput sgr0
	dist/test_dumpdot dist/examples/example0.c1 > ast0.dot
	dot -Tpng -o ast0.png ast0.dot
	dist/test_dumpdot dist/examples/example1.c1 > ast1.dot
	dot -Tpng -o ast1.png ast1.dot
	dist/test_dumpdot dist/examples/example2.c1 > ast2.dot
	dot -Tpng -o ast2.png ast2.dot
	dist/test_dumpdot dist/examples/example3.c1 > ast3.dot
	dot -Tpng -o ast3.png ast3.dot
	-rm -f *.dot

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

P5: test_dumpdot
	@echo
	@tput setaf 3; echo '* Check doc/P5.md for answers.'; tput sgr0

