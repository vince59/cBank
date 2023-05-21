# Compiler options eee
CC := gcc
CFLAGS := -Wall -Wextra -g

# Directories
SRCDIR := src
OBJDIR := obj
BINDIR := bin
LIBDIR := lib
INCDIR := include
LIBNAM := libcbank.a

# Source files
SOURCES := $(shell find $(SRCDIR) -name '*.c')
LIB_SOURCES := $(wildcard $(SRCDIR)/lib/*.c)
MAIN_SOURCES := $(filter-out $(LIB_SOURCES), $(SOURCES))

# Object files
LIB_OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(LIB_SOURCES:.c=.o))

# Binary names
EXECUTABLES := $(patsubst $(SRCDIR)/%.c,$(BINDIR)/%,$(MAIN_SOURCES))

# Library name
LIBRARY := $(LIBDIR)/$(LIBNAM)

# Include directories
INCLUDES := -I$(INCDIR)

# Default rule
all: $(EXECUTABLES)

# Rule to create each target executable
$(BINDIR)/%: $(OBJDIR)/%.o $(LIBRARY)
	@mkdir -p $(BINDIR)
	@echo "Linking $@..."
	$(CC) $(CFLAGS) -o $@ $< $(LIBRARY)

# Rule to compile object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# Rule to create the library
$(LIBRARY): $(LIB_OBJECTS)
	@mkdir -p $(LIBDIR)
	@echo "Creating library $@..."
	ar rcs $@ $^

# Clean rule
clean:
	@echo "Cleaning..."
	$(RM) -r $(OBJDIR) $(BINDIR) $(LIBDIR)

# Print variables
print-%:
	@echo $* = $($*)

.PHONY: all clean print-%