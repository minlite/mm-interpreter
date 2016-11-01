LD = g++
LDFLAGS = -lm
CC = g++
CCFLAGS = -fPIC -std=c++11

all: mm

mm:
	$(CC) $(CCFLAGS) -c *.cpp
	$(LD) $(LDFLAGS) -o mm *.o

clean:
	rm -Rf *.o *.gch mm
