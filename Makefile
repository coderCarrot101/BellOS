ASM=nasm
CC=i686-elf-gcc
LD=i686-elf-ld

SRC_DIR=src
BUILD_DIR=build

OS_IMAGE=$(BUILD_DIR)/bellos.bin

SRCS=$(wildcard $(SRC_DIR)/*.c)

C_OBJS=$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

ALL_OBJS=$(BUILD_DIR)/kernel_entry.o $(C_OBJS)

all: $(OS_IMAGE)

$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) -f bin $< -o $@

$(BUILD_DIR)/kernel_entry.o: $(SRC_DIR)/kernel_entry.asm
	mkdir -p $(BUILD_DIR)
	$(ASM) -f elf32 $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) -ffreestanding -m32 -c $< -o $@

$(BUILD_DIR)/kernel.bin: $(ALL_OBJS)
	$(LD) -T $(SRC_DIR)/linker.ld -o $@ $(ALL_OBJS) --oformat binary -Map=$(BUILD_DIR)/kernel.map

$(OS_IMAGE): $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin > $(OS_IMAGE)
	dd if=/dev/zero of=$(OS_IMAGE) bs=1 count=0 seek=51200

run: $(OS_IMAGE)
	qemu-system-i386 -drive format=raw,file=$(OS_IMAGE) -k en-us

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean