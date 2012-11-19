CXX = g++
MPICC = mpicc
temperature = temperature.o
serial = heat_serial.o
objects = $(serial) $(temperature)
objects1 = heat_omp.cpp $(temperature)
objects2 = heat_mpi.cpp $(temperature)

CXXFLAGS = -I. -O3 -Wall

all: heat_serial heat_omp heat_mpi

heat_serial : $(objects)
	$(CXX) -O3 -o $@ $^

heat_omp : $(objects1)
	$(CXX) -O3 -o $@ $^ -fopenmp

heat_mpi : $(objects2)
	$(MPICC) -O3 -o $@ $^

clean:
	$(RM) heat_serial heat_omp heat_mpi	
	$(RM) *.o
	$(RM) .depend
	$(RM) *_output
