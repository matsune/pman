CXX = g++
CXXFLAGS = -O2 -Wall
INCLUDES = -I./include
SRCDIR   = src
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)

all: clean bin/pmand

bin/pmand: $(OBJS)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) $(OBJS)
