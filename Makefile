SRC_DIR ?= src
HEADER_DIR ?= include

SOURCES := $(shell find src -name "*.cpp" -or -name "*.s")
OBJECTS := $(addsuffix .o,$(basename $(SOURCES)))
INCLUDES := $(shell find include -type d | sed s/^/-I/)

CPPC = g++
	CPPFLAGS = -Wall -Werror $(INCLUDES)

all: $(OBJECTS)
	$(CPPC) -o $@ $^ $(CPPFLAGS)

%.o: %.cpp
	$(CPPC)  $< -o $@ -lstdc++ $(CPPFLAGS)

.PHONY: clean

clean:
	rm -rf *.o
