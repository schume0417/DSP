.PHONY: all clean

CFLAGS+=-std=c++11
LDFLAGS+=-lm     # link to math library
CC = g++


all:
# type make/make all to compile test_hmm
	$(CC) train.cpp -o train
	$(CC) test.cpp -o test

clean:
	rm ./train   # type make clean to remove the compiled file
	rm ./test
