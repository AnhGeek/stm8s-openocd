# Compiler
CC = sdcc

# Linker (SDCC uses 'sdld' internally, but we specify ELF output)
LD = sdcc

# Target MCU (Modify this for your specific MCU)
MCU = -mstm8

# Output file
OUTPUT = output.elf

# Object file directory
OBJ_DIR = _out

# Find all C source files
SRCS := $(wildcard *.c)

# Generate object file paths in _out directory
OBJS := $(SRCS:%.c=$(OBJ_DIR)/%.rel)

# Default rule: Build the project
all: $(OUTPUT)

# Ensure _out directory exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile each .c file into .rel, placing output in _out/
$(OBJ_DIR)/%.rel: %.c | $(OBJ_DIR)
	$(CC) $(MCU) -c $< -o $@

# Link all object files into an ELF file inside _out/
$(OUTPUT): $(OBJS)
	$(LD) $(MCU) $(OBJS) -o $(OUTPUT)

# Clean up compiled files
clean:
	rm -rf $(OBJ_DIR) $(OUTPUT)