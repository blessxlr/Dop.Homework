.PHONY: run clean all

CXXFLAGS = -Wall -Wextra
TARGET = test

all: $(TARGET)

$(TARGET): main.o min_circle.o
	g++  main.o min_circle.o -o $(TARGET)

main.o: main.cpp
	g++  $(CXXFLAGS) -c main.cpp

min_circle.o: min_circle.cpp
	g++  $(CXXFLAGS) -c min_circle.cpp

run: all
	./$(TARGET)

clean: 
	rm -rf *.o $(TARGET)
