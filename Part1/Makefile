CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

t : t.cpp
	g++  t.cpp -o mycode -std=c++11 $(CFLAGS) $(LIBS)
	
#g++ t.cpp -o empty -std=c++11 `pkg-config --cflags --libs opencv`
