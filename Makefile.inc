CXX = g++
CXXFLAGS = -std=c++17 -Werror -Wpedantic -Wall $(CXX_EXTRA_FLAGS)
CXXFLAGS_R = $(CXXFLAGS) -O3 -DNDEBUG
CXXFLAGS_D = $(CXXFLAGS) -g  -DDEBUG
CXX_EXTRA_FLAGS += $(shell pkg-config --cflags gtest 2>/dev/null)
GTEST_LIBS = $(shell pkg-config --libs gtest_main 2>/dev/null)
ifeq ($(strip $(GTEST_LIBS)),)
GTEST_LIBS = -lgtest -lgtest_main -lpthread
endif
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
