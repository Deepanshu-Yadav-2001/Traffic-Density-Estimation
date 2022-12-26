CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

cde : cde.cpp
	g++  cde.cpp -o mycode -std=c++11 $(CFLAGS) $(LIBS)
	
#g++ cde.cpp -o empty -std=c++11 `pkg-config --cflags --libs opencv`
