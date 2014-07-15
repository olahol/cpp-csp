CFLAGS=-g -Werror -Weverything -Wno-padded -Wno-c++98-compat -std=c++11
CC=clang++

all: build build/interleave build/primes build/select

build:
	mkdir build

build/interleave: examples/interleave.cpp csp.hpp
	clang++ $(CFLAGS) examples/interleave.cpp -o build/interleave -pthread

build/primes: examples/primes.cpp csp.hpp
	clang++ $(CFLAGS) examples/primes.cpp -o build/primes -pthread

build/select: examples/select.cpp csp.hpp
	clang++ $(CFLAGS) examples/select.cpp -o build/select -pthread

clean:
	rm -r build

tidy:
	clang-format-3.4 -i csp.hpp
