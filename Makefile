# 

CC = gcc
TESTFILE = testfile.txt
TEST1 = foo Random Hello World
TEST2 = word Parola palabra
TEST3 = InTheMiddle InTheCenter InBetween
TEST4 = NotFound notFound not_found nada 

linear_kmp: linear_kmp.c kmp_search.h
	$(CC) -o $@ $<

linear_ac: linear_ac.c ac_search.h queue.h
	$(CC) -o $@ $<

all: linear_kmp linear_ac

clean:
	rm linear_kmp linear_ac

test_linear_kmp1: linear_kmp
	./linear_kmp $(TESTFILE) $(TEST1)

test_linear_kmp2: linear_kmp
	./linear_kmp $(TESTFILE) $(TEST2)

test_linear_kmp3: linear_kmp
	./linear_kmp $(TESTFILE) $(TEST3)

test_linear_kmp4: linear_kmp
	./linear_kmp $(TESTFILE) $(TEST4)

test_linear_ac1: linear_ac
	./linear_ac $(TESTFILE) $(TEST1)

test_linear_ac2: linear_ac
	./linear_ac $(TESTFILE) $(TEST2)

test_linear_ac3: linear_ac
	./linear_ac $(TESTFILE) $(TEST3)

test_linear_ac4: linear_ac_
	./linear_ac $(TESTFILE) $(TEST4)
