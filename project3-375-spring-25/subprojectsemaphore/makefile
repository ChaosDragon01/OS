CXX = g++
CXXFLAGS = -Wall -std=c++11

TARGET = main
OBJS = main.o Semaphore.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp Semaphore.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Semaphore.o: Semaphore.cpp Semaphore.h
	$(CXX) $(CXXFLAGS) -c Semaphore.cpp

clean:
	rm -f $(TARGET) $(OBJS)


