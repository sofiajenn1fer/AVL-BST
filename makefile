CXX = g++
CXXFLAGS = -Wall

driver: streak.o mytest.cpp
	$(CXX) $(CXXFLAGS) streak.o mytest.cpp -o mytest

streak.o: streak.h streak.cpp
	$(CXX) $(CXXFLAGS) -c streak.cpp

run:
	./mytest

val:
	valgrind ./mytest

clean:
	rm *.o
	rm *~
