CXX = g++
CXXFLAGS = -ggdb -std=c++17 -W -Wall -Werror 
LDFLAGS  = -lGL -lGLEW -lSDL2

TARGET = mandelbrot
SRCS   = mandelbrot.cpp program.cpp sdlapp.cpp mandelbrotapp.cpp
OBJS   = $(SRCS:.cpp=.o)
DEPS   = $(SRCS:.cpp=.depends)

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS) 

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.depends: %.cpp
	$(CXX) -MM $(CXXFLAGS) $< > $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)
