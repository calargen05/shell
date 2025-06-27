# defines the C compiler
CC = gcc

# defines the compilation flags
CFLAGS = -Wall -g

# source files
SRCS = $(wildcard *.c)

# object files
OBJS = $(SRCS:.c=.o)

# name of the executable
TARGET = shell

# phony targets
.PHONY: all clean run


# Rules

# Default rule
all: $(TARGET)
	@echo "Build complete: $(TARGET)"

# link executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
	@echo "Executable created: $(TARGET)"

# pattern rule for generating object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# clean up build projects
clean:
	@echo "Cleaning up..."
	$(RM) $(OBJS) $(TARGET) core
	@echo "Clean complete."

# Rule to run the compled program
run: $(TARGET)
	@echo "Running $(TARGET)..."
	./$(TARGET)
