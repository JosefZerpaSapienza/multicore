# 

CC = gcc
MPICC = mpicc
ALL = linear_kmp linear_ac mpi_kmp omp_kmp
TESTFILE = testfile.txt
TESTDUMP = testdump.pcap
TEST1 = foo Random Hello World
TEST2 = InTheMiddle InTheCenter InBetween
TEST3 = NotFound notFound not_found nada 

linear_kmp: linear_kmp.c kmp_search.h
	$(CC) -o $@ $<

linear_ac: linear_ac.c ac_search.h queue.h 
	$(CC) -o $@ $<

mpi_kmp: mpi_kmp.c kmp_search.h overlapped_read.h 
	$(MPICC) -o $@ $<

omp_kmp: omp_kmp.c kmp_search.h overlapped_read.h 
	$(CC) -fopenmp -o $@ $<

all: $(ALL)

clean:
	rm -f $(ALL)

testfile_linear_kmp1: linear_kmp
	./$< $(TESTFILE) $(TEST1)

testfile_linear_kmp2: linear_kmp
	./$< $(TESTFILE) $(TEST2)

testfile_linear_kmp3: linear_kmp
	./$< $(TESTFILE) $(TEST3)

testfile_linear_kmp4: linear_kmp
	./$< $(TESTFILE) $(TEST4)

testfile_linear_ac1: linear_ac
	./$< $(TESTFILE) $(TEST1)

testfile_linear_ac2: linear_ac
	./$< $(TESTFILE) $(TEST2)

testfile_linear_ac3: linear_ac
	./$< $(TESTFILE) $(TEST3)

testfile_linear_ac4: linear_ac_
	./$< $(TESTFILE) $(TEST4)

testfile_mpi_kmp1: mpi_kmp
	mpirun -np 4 ./$< $(TESTFILE) $(TEST1)

testfile_mpi_kmp2: mpi_kmp
	mpirun -np 4 ./$< $(TESTFILE) $(TEST2)
	
testfile_mpi_kmp3: mpi_kmp
	mpirun -np 4 ./$< $(TESTFILE) $(TEST3)

testfile_mpi_kmp4: mpi_kmp
	mpirun -np 4 ./$< $(TESTFILE) $(TEST4)

testfile_omp_kmp1: omp_kmp
	./$< 4 $(TESTFILE) $(TEST1)

testfile_omp_kmp2: omp_kmp
	./$< 4 $(TESTFILE) $(TEST2)

testfile_omp_kmp3: omp_kmp
	./$< 4 $(TESTFILE) $(TEST3)

testfile_omp_kmp4: omp_kmp
	./$< 4 $(TESTFILE) $(TEST4)

