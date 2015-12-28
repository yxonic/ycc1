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
test: test_parser test_kaleidoscope test_ycc1

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
	@tput setaf 3; echo '* Testing utils'; tput sgr0
	dist/test_utils
	@tput setaf 3; echo '* Some logged to test.log'; tput sgr0
	cat test.log
	rm -f test.log

test_ycc1: pre-build
	@cd dist; cmake ../; make -s ycc1
	@echo
	@tput setaf 3; echo '* Testing the main compiler'; tput sgr0

	@echo

	@tput setaf 3; echo '* Input a number and double it:'; tput sgr0
	dist/ycc1 dist/examples/cg1.c1 -S -o cg1.ll
	clang cg1.ll lib/c1lib.c -o cg1.run
	echo "32768" > ./cg1.run

	@tput setaf 3; echo '* Quick sort:'; tput sgr0
	dist/ycc1 dist/examples/cg2.c1 -S -o cg2.ll
	cat cg2.ll
	clang cg2.ll lib/c1lib.c -o cg2.run
	./cg2.run

	@tput setaf 3; echo '* Bubble sort:'; tput sgr0
	dist/ycc1 dist/examples/example2.c1 -S -o example2.ll
	clang example2.ll lib/c1lib.c -o example2.run
	./example2.run

	@tput setaf 3; echo '* Prime:'; tput sgr0
	dist/ycc1 dist/examples/example3.c1 -S -o example3.ll
	clang example3.ll lib/c1lib.c -o example3.run
	./example3.run
	rm -f *.ll *.run

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

P7: test_ycc1
	@echo
	@tput setaf 3; echo '* Check doc/P7.md for answers.'; tput sgr0
