
all: $(patsubst %.cpp, %.o, $(wildcard *.cpp))

%.o: %.cpp
	g++ -std=c++20 -fcoroutines $< -o $@

clean:
	rm -f *.o

.PHONY: all clean
