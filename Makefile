# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -std=c++20

# SDL2 flags obtained from pkg-config
SDL2_FLAGS := $(shell pkg-config --cflags --libs sdl2)

# Source files
SRC = main.cpp

# Executable name
EXE = output

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(SDL2_FLAGS)

clean:
	rm -f $(EXE)

run:
	./output

.PHONY: all clean

