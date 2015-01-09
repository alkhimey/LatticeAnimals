LINUX_CXX=g++
WINDOWS_CXX=i586-mingw32msvc-g++

COMMON_FLAGS=-Wall -O2
WINDOWS_AFTER_FLAGS=-lws2_32 # linkage only
DEBUG_FLAGS=-g


# Use ':=' instead of '=' to avoid multiple evaluation of NOW.
NOW := $(shell date)

OBJ_DIR=obj
BIN_DIR=bin

RELEASE_DIR=release
DEBUG_DIR=debug

LINUX_SUFFIX=linux
WINDOWS_SUFFIX=windows

EXE_PREFIX=client

HEADERS=$(wildcard *.h)
SRCS=$(wildcard *.cpp)
 


.PHONY: clean all

all: make_directories link_linux_release link_windows_release link_linux_debug link_windows_debug


make_directories:
	echo $(OBJ_DIR)/$(RELEASE_DIR)xxx_$(LINUX_SUFFIX).o

	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)/$(RELEASE_DIR)
	mkdir -p $(BIN_DIR)/$(RELEASE_DIR)
	mkdir -p $(OBJ_DIR)/$(DEBUG_DIR)
	mkdir -p $(BIN_DIR)/$(DEBUG_DIR)

link_linux_release: $(addprefix $(OBJ_DIR)/$(RELEASE_DIR)/, $(SRCS:.cpp=_$(LINUX_SUFFIX).o)) 
	$(LINUX_CXX)   $(COMMON_FLAGS) $^ $(AFTER_FLAGS) -o $(BIN_DIR)/$(RELEASE_DIR)/$(EXE_PREFIX)_$(LINUX_SUFFIX)

link_windows_release: $(addprefix $(OBJ_DIR)/$(RELEASE_DIR)/, $(SRCS:.cpp=_$(WINDOWS_SUFFIX).o))
	$(WINDOWS_CXX) $(COMMON_FLAGS) $^ $(WINDOWS_AFTER_FLAGS) -o $(BIN_DIR)/$(RELEASE_DIR)/$(EXE_PREFIX)_$(WINDOWS_SUFFIX)

link_linux_debug: $(addprefix $(OBJ_DIR)/$(DEBUG_DIR)/, $(SRCS:.cpp=_$(LINUX_SUFFIX).o))
	$(LINUX_CXX)    $(DEBUG_FLAGS) $(COMMON_FLAGS) $^ -o $(BIN_DIR)/$(DEBUG_DIR)/$(EXE_PREFIX)_$(LINUX_SUFFIX)

link_windows_debug: $(addprefix $(OBJ_DIR)/$(DEBUG_DIR)/, $(SRCS:.cpp=_$(WINDOWS_SUFFIX).o))
	$(WINDOWS_CXX)  $(DEBUG_FLAGS) $(COMMON_FLAGS) $^ $(WINDOWS_AFTER_FLAGS) -o $(BIN_DIR)/$(DEBUG_DIR)/$(EXE_PREFIX)_$(WINDOWS_SUFFIX)




$(OBJ_DIR)/$(RELEASE_DIR)/%_$(LINUX_SUFFIX).o: %.cpp %.h
	$(LINUX_CXX)   -c $(COMMON_FLAGS) -o $@ $<

$(OBJ_DIR)/$(RELEASE_DIR)/%_$(WINDOWS_SUFFIX).o: %.cpp %.h
	$(WINDOWS_CXX) -c $(COMMON_FLAGS) -o $@ $<

$(OBJ_DIR)/$(DEBUG_DIR)/%_$(LINUX_SUFFIX).o: %.cpp %.h
	$(LINUX_CXX)   -c $(DEBUG_FLAGS) $(COMMON_FLAGS) -o $@ $<

$(OBJ_DIR)/$(DEBUG_DIR)/%_$(WINDOWS_SUFFIX).o: %.cpp %.h
	$(WINDOWS_CXX) -c $(DEBUG_FLAGS) $(COMMON_FLAGS) -o $@ $<






clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
