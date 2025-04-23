CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
LIBS = -lSDL2 -lSDL2_mixer -lSDL2_ttf -ltag -lavformat -lavutil -lavcodec

SRCDIR = .
OBJDIR = obj
BINDIR = bin

# Find all .cpp files recursively
SRC = $(shell find $(SRCDIR) -name '*.cpp')

# Object files (replace .cpp with .o)
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))

BIN = $(BINDIR)/media_player

# Explicit rule for compiling C++ files
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Explicit rule for linking the executable
$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LIBS) -o $@

all: $(BIN)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

run: all
	./$(BIN) $(PWD)

.PHONY: all clean run