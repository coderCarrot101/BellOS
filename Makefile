ASM=nasm
CC=i686-elf-gcc
LD=i686-elf-ld

SRC_DIR=src
BUILD_DIR=build

OS_IMAGE=$(BUILD_DIR)/bellos.bin

# Automatically find all .c files in the source directory
SRCS=$(wildcard $(SRC_DIR)/*.c)

# Generate a list of corresponding object files in the build directory
# BUT isolate kernel.o so we can enforce its placement at the front
OBJS_WITHOUT_KERNEL=$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(filter-out $(SRC_DIR)/kernel.c,$(SRCS)))
ALL_OBJS=$(BUILD_DIR)/kernel.o $(OBJS_WITHOUT_KERNEL)

all: $(OS_IMAGE)

# Assemble the bootloader directly into a standalone 512-byte raw binary
$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) -f bin $< -o $@

# Pattern rule to compile any .c file into a .o file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) -ffreestanding -m32 -c $< -o $@

# Link the C kernel into a standalone flat binary raw file
# $(ALL_OBJS) ensures kernel.o is strictly the first file evaluated by the linker
$(BUILD_DIR)/kernel.bin: $(ALL_OBJS)
	$(LD) -T $(SRC_DIR)/linker.ld -o $@ $(ALL_OBJS) --oformat binary

# GLUE THEM TOGETHER & PAD THE DISK IMAGE
$(OS_IMAGE): $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	# 1. Stitch bootloader and kernel binary together
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin > $(OS_IMAGE)
	# 2. Force the disk image to be padded with zeros up to 4 sectors (2048 bytes total)
	dd if=/dev/zero of=$(OS_IMAGE) bs=1 count=0 seek=2048

run: $(OS_IMAGE)
	qemu-system-i386 -drive format=raw,file=$(OS_IMAGE)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean