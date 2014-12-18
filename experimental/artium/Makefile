CFLAGS=-c -Wall -O2
CXX=gcc


all: *.o
	$(CXX) *.o -o art


%.o: %.c %.h
	$(CXX) $(CFLAGS) $< -o $@
