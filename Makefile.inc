CXX = g++
GTEST_PREFIX = /opt/homebrew/opt/googletest
CXXFLAGS = -std=c++17 -Werror -Wpedantic -Wall -I$(GTEST_PREFIX)/include
CXXFLAGS_R = $(CXXFLAGS) -O3 -DNDEBUG
CXXFLAGS_D = $(CXXFLAGS) -g  -DDEBUG
GTEST_LIBS = -L$(GTEST_PREFIX)/lib -lgtest -lgtest_main -lpthread
LD=$(CXX)

%.d.o : %.cpp
	$(CXX) -c $(CXXFLAGS_D) -o $@ $<

%.r.o : %.cpp
	$(CXX) -c $(CXXFLAGS_R) -o $@ $<

clean :
	- rm *.o
	- rm *.hdrdep.bak

cleanall : clean
	- rm release debug debug_test utest
	- echo > Makefile.hdrdep

mrproper : cleanall

astyle:
	astyle --options=../astyle.ini *.h *.cpp

tidy:
	../tidy.sh

depends :
	makedepend -f Makefile.hdrdep *.c*
