HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
CXXFLAGS = -O2 -Wall -std=c++11
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
BIN_DIR = bin
TARGET = $(BIN_DIR)/pman
SUBMODULES_DIR = submodules
LIB_DIR = lib
ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`\
           -lgrpc++_reflection\
           -ldl
else
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl
endif

# inih
INI_SRC_DIR = $(SUBMODULES_DIR)/inih
INI_SRCS = $(INI_SRC_DIR)/ini.cpp \
						$(INI_SRC_DIR)/cpp/INIReader.cpp
INI_OBJS = $(INI_SRCS:.cpp=.o)

# gRPC
PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`
PROTOS_PATH = protos
PROTO_OBJS = $(SRC_DIR)/pman.pb.o \
 						$(SRC_DIR)/pman.grpc.pb.o

vpath %.proto $(PROTOS_PATH)

.PHONY: all
all: system-check $(TARGET)

$(TARGET): $(INI_OBJS) $(PROTO_OBJS) $(OBJS)
	@[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)
	$(CXX) $(INCLUDES) $(LDFLAGS) -o $@ $^

.PHONY: protos
protos: $(PROTO_OBJS)

.PHONY: clean
clean:
	$(RM) $(SRC_DIR)/*.o $(INI_OBJS) $(TARGET) $(SRC_DIR)/*.pb.*

.PHONY: clean-log
clean-log:
	$(RM) *.log

.PRECIOUS: %.grpc.pb.cc
$(SRC_DIR)/%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=$(SRC_DIR) --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

.PRECIOUS: %.pb.cc
$(SRC_DIR)/%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=$(SRC_DIR) $<

# The following is to test your system and ensure a smoother experience.
# They are by no means necessary to actually compile a grpc-enabled software.

PROTOC_CMD = which $(PROTOC)
PROTOC_CHECK_CMD = $(PROTOC) --version | grep -q libprotoc.3
PLUGIN_CHECK_CMD = which $(GRPC_CPP_PLUGIN)
HAS_PROTOC = $(shell $(PROTOC_CMD) > /dev/null && echo true || echo false)
ifeq ($(HAS_PROTOC),true)
HAS_VALID_PROTOC = $(shell $(PROTOC_CHECK_CMD) 2> /dev/null && echo true || echo false)
endif
HAS_PLUGIN = $(shell $(PLUGIN_CHECK_CMD) > /dev/null && echo true || echo false)

SYSTEM_OK = false
ifeq ($(HAS_VALID_PROTOC),true)
ifeq ($(HAS_PLUGIN),true)
SYSTEM_OK = true
endif
endif

system-check:
ifneq ($(HAS_VALID_PROTOC),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have protoc 3.0.0 installed in your path."
	@echo "Please install Google protocol buffers 3.0.0 and its compiler."
	@echo "You can find it here:"
	@echo
	@echo "   https://github.com/google/protobuf/releases/tag/v3.0.0"
	@echo
	@echo "Here is what I get when trying to evaluate your version of protoc:"
	@echo
	-$(PROTOC) --version
	@echo
	@echo
endif
ifneq ($(HAS_PLUGIN),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have the grpc c++ protobuf plugin installed in your path."
	@echo "Please install grpc. You can find it here:"
	@echo
	@echo "   https://github.com/grpc/grpc"
	@echo
	@echo "Here is what I get when trying to detect if you have the plugin:"
	@echo
	-which $(GRPC_CPP_PLUGIN)
	@echo
	@echo
endif
ifneq ($(SYSTEM_OK),true)
	@false
endif

#
# test
#
TEST_DIR = test
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)
TEST_TARGET = $(BIN_DIR)/pman_test
GTEST_DIR = $(SUBMODULES_DIR)/googletest/googletest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
								$(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
TEST_FLAGS += -isystem $(GTEST_DIR)/include


$(TEST_DIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS)$(TEST_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c -o $@ $(GTEST_DIR)/src/gtest-all.cc

$(TEST_DIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS)$(TEST_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c -o $@ $(GTEST_DIR)/src/gtest_main.cc

$(LIB_DIR)/gtest.a : $(TEST_DIR)/gtest-all.o
	@[ -d $(LIB_DIR) ] || mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_DIR)/gtest_main.a : $(TEST_DIR)/gtest-all.o $(TEST_DIR)/gtest_main.o
	@[ -d $(LIB_DIR) ] || mkdir -p $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

.PHONY: test
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(INI_OBJS) $(TEST_OBJS) $(LIB_DIR)/gtest_main.a
	@[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $(TEST_OBJS) $(LIB_DIR)/gtest_main.a $(LIBS) -lpthread \
		src/cmd_parser.o src/util.o $(INI_OBJS) src/conf_parser.o

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS)$(TEST_FLAGS) $(CXXFLAGS) $(INCS) -o $@ -c $<

.PHONY: clean-test
clean-test:
	$(RM) $(TEST_TARGET) $(TEST_OBJS)
