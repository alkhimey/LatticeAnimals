CFLAGS=-c -Wall -O2

# Use ':=' instead of '=' to avoid multiple evaluation of NOW.
NOW := $(shell date)

OBJ_DIR=obj
BIN_DIR=bin

LINUX_NAME=redelemeier-linux
WIN_NAME=redelemeier-win.exe

HEADERS=$(wildcard *.h)
SRCS=$(wildcard *.cpp)
OBJS=$(addprefix $(OBJ_DIR)/, $(filter-out main.o, $(SRCS:.cpp=.o)))

LINUX_BINARY=$(BIN_DIR)/$(LINUX_NAME)


.PHONY: clean all

all: set_linux_vars make_directories linkage
	echo $(NOW)	


make_directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

set_linux_vars:
#	$(eval BINARY_NAME:=$(LINUX_NAME))
	$(eval CXX:=g++)
#	$(eval OBJ_SUFFIX:=linux.o)


#set_windows_vars:
#	$(eval CXX:=i586-mingw32msvc-g++)
#	$(eval OBJ_SUFFIX:=win.o)
#	$(eval AFTER_FLAGS:=-lws2_32)


linkage: $(OBJS) main.cpp
	$(CXX) main.cpp $(OBJS) $(AFTER_FLAGS) -o $(LINUX_BINARY)

#$(OBJ_DIR)/main.o: main.cpp
#	$(CXX) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: %.cpp %.h
	$(CXX) $(CFLAGS) -o $@ $<



clean:
	rm -rf $(OBJS) $(LINUX_BINARY)
