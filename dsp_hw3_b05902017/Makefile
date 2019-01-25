SRIPATH ?=/home/student/05/b05902017/dsp/hw3/srilm-1.5.10

MACHINE_TYPE ?= i686-m64

CXX=g++
CXXFLAGS=-O3 -I$(SRIPATH)/include -w --std=c++11

vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)

TARGET=mydisambig
SRC = mydisambig.cpp
OBJ = $(SRC:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ) -loolm -ldstruct -lmisc
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

run:
	./mydisambig -lm bigram.lm -map ZhuYin-Big5.map -text testdata/1.txt > result2/1.txt
	./mydisambig -lm bigram.lm -map ZhuYin-Big5.map -text testdata/2.txt > result2/2.txt
	./mydisambig -lm bigram.lm -map ZhuYin-Big5.map -text testdata/3.txt > result2/3.txt
	./mydisambig -lm bigram.lm -map ZhuYin-Big5.map -text testdata/4.txt > result2/4.txt
	./mydisambig -lm bigram.lm -map ZhuYin-Big5.map -text testdata/5.txt > result2/5.txt
	./mydisambig -lm bigram.lm -map ZhuYin-Big5.map -text testdata/6.txt > result2/6.txt
	./mydisambig -lm bigram.lm -map ZhuYin-Big5.map -text testdata/7.txt > result2/7.txt
	./mydisambig -lm bigram.lm -map ZhuYin-Big5.map -text testdata/8.txt > result2/8.txt
	./mydisambig -lm bigram.lm -map ZhuYin-Big5.map -text testdata/9.txt > result2/9.txt
	./mydisambig -lm bigram.lm -map ZhuYin-Big5.map -text testdata/10.txt > result2/10.txt

map:
	python3 mapping.py

clean:
	$(RM) $(OBJ) $(TARGET)


