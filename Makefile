# Compiler and Linker
CC = sdcc
LD = sdcc

# Target MCU (Modify for your specific MCU)
MCU = -mstm8
CFLAGS = --out-fmt-elf --all-callee-saves --verbose --stack-auto --fverbose-asm  --float-reent --no-peep --debug
# Output directory
OUT_DIR = _out

# ELF output (inside _out/)
# Output files
ELF_OUTPUT = $(OUT_DIR)/output.elf
HEX_OUTPUT = $(OUT_DIR)/output.hex

# Find all C source files
SRCS := $(wildcard *.c)

# Generate object file paths in _out directory
OBJS := $(SRCS:%.c=$(OUT_DIR)/%.rel)

# Default rule: Build everything
all: $(HEX_OUTPUT) $(ELF_OUTPUT)

# Ensure _out directory exists
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Compile each .c file into .rel, placing output in _out/
$(OUT_DIR)/%.rel: %.c | $(OUT_DIR)
	$(CC) $(MCU) -c $< $(CFLAGS) -o $@

# Link all object files into IHX format (Intel HEX)
$(HEX_OUTPUT): $(OBJS)
	$(LD) $(MCU) $(CFLAGS) $(OBJS) -o $(OUT_DIR)/output.ihx
	mv $(OUT_DIR)/output.ihx $(HEX_OUTPUT)

# Convert HEX to ELF using objcopy
$(ELF_OUTPUT): $(OBJS)
	cd $(OUT_DIR) && $(LD) $(MCU) $(notdir $(OBJS)) --out-fmt-elf -o output.elf

# Clean up compiled files
clean:
	rm -rf $(OUT_DIR)