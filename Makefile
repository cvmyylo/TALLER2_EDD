CXX = g++
CXXFLAGS = -std=c++14 -Wall

SRCS = main.cpp classes/Cancion.cpp core/GestorArchivos.cpp core/ControladorReproductor.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = reproductor

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)