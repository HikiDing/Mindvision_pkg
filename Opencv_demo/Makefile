CXX ?= g++
CFLAGS =`pkg-config --cflags opencv4`
LIBS = `pkg-config --libs opencv4`
INCLUDE= -I../../include

all: main
main:  
	$(CXX) $(CFLAGS)  -o test test.cpp $(LIBS)   $(INCLUDE)   -lMIVISDK -L /lib/
clean:
	rm -f *.o 
	rm -f main 
	