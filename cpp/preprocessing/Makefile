all: dc

dc: datachange.cpp
	g++ -O2 -o dc datachange.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv`
