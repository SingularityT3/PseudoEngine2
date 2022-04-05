CXX = g++
CXX_FLAGS = -Wall -Werror -Isrc -std=c++20
DEBUG_FLAGS = -g
RELEASE_FLAGS = -O2

SRCS = $(shell find src/ -type f -name '*.cpp')
OBJS = $(patsubst src/%.cpp, obj/%.o, $(SRCS))
BIN = bin/PSE2

all: debug

debug: CXX_FLAGS += $(DEBUG_FLAGS)
debug: $(BIN)

release: CXX_FLAGS += $(RELEASE_FLAGS)
release: $(BIN)

$(BIN): $(OBJS)
	@mkdir -p bin
	$(CXX) $(CXX_FLAGS) $(OBJS) -o $(BIN)

obj/%.o: src/%.cpp
	@p="$@" ; b=`basename $$p` ; f=$${p%$$b} ; mkdir -p $$f
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf bin/* obj/*
