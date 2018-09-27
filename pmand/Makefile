CXX = g++
CXXFLAGS = -O2 -Wall -std=c++11
SRCDIR   = src
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = bin/pmand

INI_SRCDIR = lib/inih
INI_SRCS = $(INI_SRCDIR)/ini.cpp \
						$(INI_SRCDIR)/cpp/INIReader.cpp
INI_OBJS = $(INI_SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS) $(INI_OBJS)
	$(CXX) $(INCLUDES) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	$(RM) $(OBJS) $(INI_OBJS) $(TARGET)
