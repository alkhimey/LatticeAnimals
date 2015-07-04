
SRC_DIR=src
SRCS=$(wildcard $(SRC_DIR)/*.cpp)
UNIT_TESTS_DIR=unit_tests
UNIT_TESTS_SRCS=$(wildcard $(UNIT_TESTS_DIR)/*.cpp)
OBJ_DIR=obj
BIN_DIR=bin

#linux_debug: CXX=g++
#linux_debug: CC_FLAGS=-c -Wall -g
#linux_debug: LD_FLAGS=
#linux_debug: SUFFIX=linux_debug
#linux_debug: EXE=$(BIN_DIR)/run_$(SUFFIX)
#linux_debug: OBJS=$(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%_$(SUFFIX).o)

#linux_debug: dirs $(EXE)



CXX=g++
CC_FLAGS=-c -Wall -g
LD_FLAGS=
SUFFIX=linux_debug

EXE=$(BIN_DIR)/run_$(SUFFIX)
UNIT_TESTS_EXE=$(BIN_DIR)/unit_tests_$(SUFFIX)
OBJS=$(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%_$(SUFFIX).o)

OBJS_WITHOUT_MAIN=$(filter-out $(OBJ_DIR)/main_$(SUFFIX).o, $(OBJS))
UNIT_TESTS_OBJS=$(UNIT_TESTS_SRCS:$(UNIT_TESTS_DIR)/%.cpp=$(OBJ_DIR)/$(UNIT_TESTS_DIR)/%_$(SUFFIX).o)


.PHONY: all clean #linux_debug



all: dirs $(EXE) $(UNIT_TESTS_EXE)


# Link only if dependency changed
#
$(EXE): $(OBJS)
	$(CXX) $(LD_FLAGS) -o $@ $^


# Similarly, link unit tests (exclude main from link)
#
$(UNIT_TESTS_EXE): $(OBJS_WITHOUT_MAIN) $(UNIT_TESTS_OBJS)
	$(CXX) $(LD_FLAGS) -o $@ $^



# Create directories. No error if directory already exists
#
dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)/$(UNIT_TESTS_DIR)

# Compile all sources and generate dependency lists
#
$(OBJ_DIR)/%_$(SUFFIX).o: $(SRC_DIR)/%.cpp
	g++ -c -MMD -MP -o $@ $<

# Compile all unit tests and generate dependency lists
#
$(OBJ_DIR)/$(UNIT_TESTS_DIR)/%_$(SUFFIX).o: $(UNIT_TESTS_DIR)/%.cpp
	g++ -c -MMD -MP -o $@ $<


# Include previously generated dependency lists.
# First time build will build all sources anyway.
#
-include $(OBJ_DIR)/*.d


# Clean
#
clean:
	rm -rf obj bin
