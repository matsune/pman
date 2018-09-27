CXX = g++
CXXFLAGS = -O2 -Wall -std=c++11
SRCDIR   = src
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = bin/pmand

all: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJS) $(TARGET)
