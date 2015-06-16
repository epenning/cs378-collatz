FILES :=                              \
    .travis.yml                       \
    collatz-tests/elp692-RunCollatz.in   \
    collatz-tests/elp692-RunCollatz.out  \
    collatz-tests/elp692-TestCollatz.c++ \
    collatz-tests/elp692-TestCollatz.out \
    Collatz.c++                       \
    Collatz.h                         \
    Collatz.log                       \
    html                              \
    RunCollatz.c++                    \
    RunCollatz.in                     \
    RunCollatz.out                    \
    TestCollatz.c++                   \
    TestCollatz.out

ifeq ($(CXX), clang++)
    COVFLAGS := --coverage
    GCOV     := gcov-4.6
else
    CXX      := g++-4.8
    COVFLAGS := -fprofile-arcs -ftest-coverage
    GCOV     := gcov-4.8
endif

# flag -DCACHE for enabling cache, -DOPTIMIZE for enabling optimization
CXXFLAGS := -pedantic -std=c++11 -Wall -DCACHE -DOPTIMIZE
LDFLAGS  := -lgtest -lgtest_main -pthread
VALGRIND := valgrind

all: RunCollatz TestCollatz

check:
	@for i in $(FILES);                                         \
	do                                                          \
        [ -e $$i ] && echo "$$i found" || echo "$$i NOT FOUND"; \
    done

clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f RunCollatz
	rm -f RunCollatz.out
	rm -f TestCollatz
	rm -f TestCollatz.out

config:
	git config -l

test: RunCollatz.out TestCollatz.out

collatz-tests:
	git clone https://github.com/cs378-summer-2015/collatz-tests.git

html: Doxyfile Collatz.h Collatz.c++ RunCollatz.c++ TestCollatz.c++
	doxygen Doxyfile

Collatz.log:
	git log > Collatz.log

Doxyfile:
	doxygen -g

RunCollatz: Collatz.h Collatz.c++ RunCollatz.c++
	$(CXX) $(CXXFLAGS) Collatz.c++ RunCollatz.c++ -o RunCollatz
	
winRunCollatz: Collatz.h Collatz.c++ RunCollatz.c++
	g++ $(CXXFLAGS) Collatz.c++ RunCollatz.c++ -o RunCollatz.exe

RunCollatz.out: RunCollatz
	cat RunCollatz.in
	./RunCollatz < RunCollatz.in > RunCollatz.out
	cat RunCollatz.out
	
winRunCollatz.out: winRunCollatz
	cat RunCollatz.in
	./RunCollatz.exe < RunCollatz.in > RunCollatz.out
	cat RunCollatz.out

TestCollatz: Collatz.h Collatz.c++ TestCollatz.c++
	$(CXX) $(COVFLAGS) $(CXXFLAGS) Collatz.c++ TestCollatz.c++ -o TestCollatz $(LDFLAGS)

TestCollatz.out: TestCollatz
	$(VALGRIND) ./TestCollatz  >  TestCollatz.out 2>&1
	$(GCOV) -b Collatz.c++     >> TestCollatz.out
	$(GCOV) -b TestCollatz.c++ >> TestCollatz.out
	cat TestCollatz.out
