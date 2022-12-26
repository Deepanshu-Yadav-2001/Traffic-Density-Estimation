CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

method1: method1.cpp
	g++  method1.cpp -o method1 -std=c++11 $(CFLAGS) $(LIBS)
	
method2: method2.cpp
	g++  method2.cpp -o method2 -std=c++11 $(CFLAGS) $(LIBS)
	
method3d: method3d.cpp
	g++  method3d.cpp -o method3d -pthread -std=c++11 $(CFLAGS) $(LIBS)
	
method3q: method3q.cpp
	g++  method3q.cpp -o method3q -pthread -std=c++11 $(CFLAGS) $(LIBS)

method4d: method4d.cpp
	g++  method4d.cpp -o method4d -pthread -std=c++11 $(CFLAGS) $(LIBS)
	
method4q: method4q.cpp
	g++  method4q.cpp -o method4q -pthread -std=c++11 $(CFLAGS) $(LIBS)

all: method1.cpp method2.cpp method3d.cpp method3q.cpp method4d.cpp method4q.cpp
	g++  method1.cpp -o method1 -std=c++11 $(CFLAGS) $(LIBS)
	g++  method2.cpp -o method2 -std=c++11 $(CFLAGS) $(LIBS)
	g++  method3d.cpp -o method3d -pthread -std=c++11 $(CFLAGS) $(LIBS)
	g++  method3q.cpp -o method3q -pthread -std=c++11 $(CFLAGS) $(LIBS)
	g++  method4d.cpp -o method4d -pthread -std=c++11 $(CFLAGS) $(LIBS)
	g++  method4q.cpp -o method4q -pthread -std=c++11 $(CFLAGS) $(LIBS)
	
#g++ t.cpp -o empty -std=c++11 `pkg-config --cflags --libs opencv`
