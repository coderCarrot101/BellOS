ASM=nasm
CC=i686-elf-gcc
LD=i686-elf-ld

SRC_DIR=src
BUILD_DIR=build

OS_IMAGE=$(BUILD_DIR)/bellos.bin

# Automatically find all .c files in the source directory
SRCS=$(wildcard $(SRC_DIR)/*.c)

# Generate a list of corresponding C object files
C_OBJS=$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# FORCE kernel_entry.o to be the absolute first file evaluated by the linker
ALL_OBJS=$(BUILD_DIR)/kernel_entry.o $(C_OBJS)

all: $(OS_IMAGE)

# Assemble the bootloader directly into a standalone 512-byte raw binary
$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) -f bin $< -o $@

# Rule to assemble kernel_entry.asm into an ELF object file
$(BUILD_DIR)/kernel_entry.o: $(SRC_DIR)/kernel_entry.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) -f elf32 $< -o $@

# Pattern rule to compile any .c file into a .o file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) -ffreestanding -m32 -c $< -o $@

# Link everything together, ensuring kernel_entry.o sits at the exact start of the binary
$(BUILD_DIR)/kernel.bin: $(ALL_OBJS)
	$(LD) -T $(SRC_DIR)/linker.ld -o $@ $(ALL_OBJS) --oformat binary -Map=$(BUILD_DIR)/kernel.map

# GLUE THEM TOGETHER & PAD THE DISK IMAGE
$(OS_IMAGE): $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin > $(OS_IMAGE)
	dd if=/dev/zero of=$(OS_IMAGE) bs=1 count=0 seek=51200

run: $(OS_IMAGE)
	qemu-system-i386 -drive format=raw,file=$(OS_IMAGE) -k en-us

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean