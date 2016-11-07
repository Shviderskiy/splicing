CXX = g++-5
LD  = g++-5

WARNINGS = -Wall -Weffc++ -Wextra -pedantic
CXX_FLAGS = -std=c++11 -masm=intel $(WARNINGS) # -m32 
LD_FLAGS  = # -m32


all: clean foo lib

foo: foo.cpp
	$(CXX) $(CXX_FLAGS) -std=c++11 foo.cpp -o foo

lib: lib.so

clean:
	rm -rf ../objects foo lib.so

all: clean foo lib

run: foo lib
	LD_PRELOAD=./lib.so ./foo
	


../objects/lib.o: ../sources/lib.cpp ../sources/lib.hpp
	@ mkdir -p ../objects
	$(CXX) $(CXX_FLAGS) -fPIC -c ../sources/lib.cpp -o ../objects/lib.o

../objects/init.o: init.cpp ../sources/lib.hpp
	@ mkdir -p ../objects
	$(CXX) $(CXX_FLAGS) -fPIC -c init.cpp -o ../objects/init.o

lib.so: ../objects/lib.o ../objects/init.o
	$(LD) $(LD_FLAGS) -shared ../objects/lib.o ../objects/init.o -o lib.so
	strip lib.so


