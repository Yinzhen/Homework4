temperature = temperature.o 
serial = heat_serial.o 
objects = $(serial) $(temperature)

CXXFLAGS = -O3 -Wall 

all: heat_serial 

heat_serial : $(objects)
	$(CXX) -O3 -o $@ $^

clean:
	$(RM) *.o
	$(RM) .depend
	$(RM) *_output

depend:
	$(CXX) -MM $(CXXFLAGS) *.cc > .depend

-include .depend
