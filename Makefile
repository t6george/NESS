SRC_DIR ?= src
HEADER_DIR ?= include

SOURCES := $(shell find src -name "*.cpp" -or -name "*.s")
OBJECTS := $(addsuffix .o,$(basename $(SOURCES)))
INCLUDES := $(shell find include -type d | sed s/^/-I/)

CPPC := g++
CPPFLAGS := -std=c++17 -Wall -Werror $(INCLUDES)

nes: $(OBJECTS)
	$(CPPC) -o $@ $^ -lstdc++ $(CPPFLAGS)

%.o: %.cpp
	$(CPPC)  $< -o $@ $(CPPFLAGS) -c

.PHONY: clean

clean:
	find . -type f -name '*.o' -delete
	rm nes
