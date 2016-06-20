# Build the project by calling:
# 
# make CFG=release PLAT=linux
# 
# CFG can be "debug" (default) or "release".
# PLAT can be "linux" (default) or "windows".
#
#



# Compiler related common configurations
#
CC_FLAGS := -c -MMD -MP -Wall -std=c++0x
LD_FLAGS= -std=c++0x
LD_LIBS= 



# makefile commands line parameters
#
ifeq ($(PLAT), windows)

#CXX=i586-mingw32msvc-g++
#CXXx86_64-w64-mingw32-g++ # requires -m32 flag
CXX=i686-w64-mingw32-g++
SUFFIX := win
LD_LIBS += -lws2_32
LD_FLAGS += -static-libgcc -static-libstdc++

else

CXX=g++
SUFFIX := linux

endif


ifeq ($(CFG), release)

CC_FLAGS += -g -O3 -DNDEBUG
LD_FLAGS += -DNDEBUG
SUFFIX := $(SUFFIX)_release

else

CC_FLAGS += -g -O0 -fno-inline-functions -D_GLIBCXX_DEBUG
SUFFIX:=$(SUFFIX)_debug

endif




# Path related configurations
#
SRC_DIR=src
SRCS=$(wildcard $(SRC_DIR)/*.cpp)
UNIT_TESTS_DIR=unit_tests
UNIT_TESTS_SRCS=$(wildcard $(UNIT_TESTS_DIR)/*.cpp)
OBJ_DIR=obj
BIN_DIR=bin
INC_DIR=include

EXE=$(BIN_DIR)/run_$(SUFFIX)
UNIT_TESTS_EXE=$(BIN_DIR)/unit_tests_$(SUFFIX)
OBJS=$(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%_$(SUFFIX).o)

OBJS_WITHOUT_MAIN=$(filter-out $(OBJ_DIR)/main_$(SUFFIX).o, $(OBJS))
UNIT_TESTS_OBJS=$(UNIT_TESTS_SRCS:$(UNIT_TESTS_DIR)/%.cpp=$(OBJ_DIR)/$(UNIT_TESTS_DIR)/%_$(SUFFIX).o)



.PHONY: all clean

#all: dirs $(EXE) $(UNIT_TESTS_EXE) $(BIN_DIR)/log4cxx.properties
all: dirs $(EXE) $(UNIT_TESTS_EXE)

# Link only if dependency changed
#
$(EXE): $(OBJS)
	$(CXX) $(LD_FLAGS) -o $@ $^ $(LD_LIBS)


# Similarly, link unit tests (exclude main from link)
#
$(UNIT_TESTS_EXE): $(OBJS_WITHOUT_MAIN) $(UNIT_TESTS_OBJS)
	$(CXX) $(LD_FLAGS) -o $@ $^ $(LD_LIBS)


#$(BIN_DIR)/log4cxx.properties: log4cxx.properties
#	cp -f log4cxx.properties $(BIN_DIR)/log4cxx.properties


# Create directories. No error if directory already exists
#
dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)/$(UNIT_TESTS_DIR)

# Compile all sources and generate dependency lists
#
$(OBJ_DIR)/%_$(SUFFIX).o: $(SRC_DIR)/%.cpp
	$(CXX) $(CC_FLAGS) -I$(INC_DIR) -o $@ $<

# Compile all unit tests and generate dependency lists
#
$(OBJ_DIR)/$(UNIT_TESTS_DIR)/%_$(SUFFIX).o: $(UNIT_TESTS_DIR)/%.cpp
	$(CXX) $(CC_FLAGS) -I$(INC_DIR) -o $@ $<


# Include previously generated dependency lists.
# First time build will build all sources anyway.
#
-include $(OBJ_DIR)/*.d


# Clean
#
clean:
	rm -rf obj bin
