#Makefile

#CVINC	=	`pkg-config --cflags opencv-3.2.0`
#CVLIB	=	`pkg-config --libs opencv-3.2.0`
CVINC	=	`pkg-config --cflags opencv`
CVLIB	=	`pkg-config --libs opencv`

CFLAGS		=	-std=c++14 -g -Wall -O2 $(CVINC)
CXX		=	g++
TARGET		=	./run
OBJS		=	main.o \
			alcon.o \
			myAlgorithm.o

.SUFFIXES: .cpp .o

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CVLIB)

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

clean:
	$(RM) -f *~ $(OBJS) $(TARGET) *.o

main.o: alcon.hpp alcon.cpp myAlgorithm.hpp
alcon.o: alcon.hpp
myAlgorithm.o: alcon.cpp alcon.hpp myAlgorithm.hpp
