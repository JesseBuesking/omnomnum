LEMON=~/repositories/lemon/lemon
# I installed via apt
RE2C=re2c
# I pulled the file from http://www.hwaci.com/sw/mkhdr/makeheaders.c here, then
# ran `gcc makeheaders.c -o makeheaders`
MAKEHEADERS=~/repositories/makeheaders/makeheaders

$(CC)=gcc
$(CXX)=g++

GTEST_DIR=$(HOME)/repositories/googletest/googletest
NAME=omnomnum

OPTIMIZATION?=-O3

STD=-std=c99 -pedantic
WARN=-Wall -W -Wno-missing-field-initializers
OPT=$(OPTIMIZATION) -msse4.2

FINAL_CFLAGS=$(STD) $(WARN) $(OPT) $(DEBUG)
FINAL_LDFLAGS=$(LDFLAGS) $(DEBUG)
FINAL_LIBS=-lm
#DEBUG=-g -ggdb -Dprint_errors
#DEBUG=-g -ggdb -Dprint_errors -Ddebug

OMNOMNUM_CC=$(CC) $(FINAL_CFLAGS)
OMNOMNUM_LD=$(CC) $(FINAL_LDFLAGS)
OMNOMNUM_CXX=$(CXX) $(WARN) $(OPT) $(DEBUG)

OMNOMNUM_OBJ=parser.o omnomnum.o scanner.o scan.o sds.o itoa.o dtoa.o scanner.def.o util.o grisu2/grisu2.o branchlut/branchlut.o
DEPS=parser.h scan.h omnomnum.h scanner.h

test/cases.yaml: ;
test/remove_char_inplace.yaml: ;

branchlut/branchlut.o: branchlut/branchlut.c $(DEPS)
	$(OMNOMNUM_CC) -c $< -o branchlut/branchlut.o

grisu2/grisu2.o: grisu2/grisu2.c $(DEPS)
	$(OMNOMNUM_CC) -c $< -o grisu2/grisu2.o

%.o: %.c $(DEPS)
	$(OMNOMNUM_CC) -c $<

all: omnomnum

omnomnum: $(OMNOMNUM_OBJ) main.o
	$(OMNOMNUM_LD) $^ -o $@ $(FINAL_LIBS)
	#./omnomnum

parser.c: parser.yy scanner.re
	# produces parser.c
	$(LEMON) -m parser.yy

parser.h: parser.c
	$(MAKEHEADERS) parser.c
	# add missing defines required by methods in parser.h
	sed -i "2i #include <stdio.h>\n#ifndef YYMALLOCARGTYPE\n# define YYMALLOCARGTYPE size_t\n#endif" parser.h

parser.o: parser.h
	# produces parser.o
	$(OMNOMNUM_CC) -c parser.c

scanner.c: scanner.re parser.yy
	#$(RE2C) -d -o $@ scanner.re
	$(RE2C) -o $@ scanner.re

clean:
	rm -rf *.o test/*.o parser.h parser.out parser.c scanner.o scanner.c main.o omnomnum *.gch test/test_omnomnum test/test_benchmark

.PHONY: all clean

test/test_omnomnum.o: parser.h scan.h omnomnum.h scanner.h test/test_omnomnum.c
	$(OMNOMNUM_CXX) -std=c++11 -L/usr/local/include -I$(GTEST_DIR)/include -c test/test_omnomnum.c -o $@ -lyaml-cpp

test/test_omnomnum: $(OMNOMNUM_OBJ) $(GTEST_DIR)/make/gtest_main.a test/test_omnomnum.o
	$(OMNOMNUM_CXX) -std=c++11 -L/usr/local/include -o $@ -I$(GTEST_DIR)/include -I. $^ -pthread -lyaml-cpp

test/test_util.o: util.h test/test_util.c
	$(CXX) -std=c++11 -L/usr/local/include -I$(GTEST_DIR)/include -c test/test_util.c -o $@ -lyaml-cpp

test/test_util: util.o $(GTEST_DIR)/make/gtest_main.a test/test_util.o
	$(CXX) -std=c++11 -L/usr/local/include -o $@ -I$(GTEST_DIR)/include -I. $^ -pthread -lyaml-cpp

test_util: test/test_util test/remove_char_inplace.yaml
	cd test && ./test_util

test: all test/test_omnomnum test/cases.yaml
	cd test && ./test_omnomnum

test/test_benchmark.o: parser.h scan.h omnomnum.h scanner.h test/test_benchmark.c
	$(CXX) -O3 -msse4.2 -std=c++11 -L/usr/local/include -c test/test_benchmark.c -o $@ -lyaml-cpp

test/test_benchmark: $(OMNOMNUM_OBJ) test/test_benchmark.o
	$(CXX) -O3 -msse4.2 -std=c++11 -L/usr/local/include -o $@ -I. $^ -pthread -lyaml-cpp -lbenchmark

benchmark: all test/test_benchmark
	cd test && ./test_benchmark
