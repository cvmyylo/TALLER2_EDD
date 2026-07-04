CXX = g++
CXXFLAGS = -std=c++14 -Wall

SRCS = main.cpp $(wildcard classes/*.cpp) $(wildcard core/*.cpp) $(wildcard data_structures/*.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = reproductor

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)