export CC=clang
export CXX=clang++
export CXXFLAGS = -Wno-deprecated-register -Wall
all: pre-build
	@echo 'Building with CMake...'
	@cd dist; cmake ../; make -s
	@echo 'Now you can run "make test" to run tests.'

.PHONY: pre-build
pre-build:
	@mkdir -p dist

.PHONY: test clean
test: test_parser test_kaleidoscope

clean:
	-rm -rf dist
	-rm -f *.png

.PHONY: test_kaleidoscope
test_kaleidoscope: pre-build
	@cd dist; cmake ../; make -s kaleidoscope
	@echo
	@tput setaf 3; echo '* Test input files are in test/examples/. Output files are test*.png.'; tput sgr0
	dist/kaleidoscope < dist/examples/kaleidoscope_ex1.k 2&>/dev/null
	dot toy.dot -Tpng -o test1.png
	dist/kaleidoscope < dist/examples/kaleidoscope_ex2.k 2&>/dev/null
	dot toy.dot -Tpng -o test2.png
	-rm toy.dot

test_parser:
	@cd dist; cmake ../; make -s test_parser
	@echo
	@tput setaf 3; echo '* Run dist/test_parser'; tput sgr0

# Targets for TA
P1:
	@echo 'Please checkout to tag P1 to run tests for P1.'

P2:
	@echo 'Please checkout to tag P2 to run tests for P2.'

P3: test_kaleidoscope
	@echo
	@tput setaf 3; echo '* Check doc/P3.md for answers.'; tput sgr0
