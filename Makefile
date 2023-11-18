BUILD_DIR = build
include $(N64_INST)/include/n64.mk

FILESYSTEM_DIR = filesystem

SOURCES = main.c
MODELS = assets/Catherine_2.8.glb

OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)
MODEL64S = $(MODELS:assets/%.glb=$(FILESYSTEM_DIR)/%.model64)

ASSETS = $(MODEL64S)

all: skinning-test.z64

$(FILESYSTEM_DIR)/%.model64: assets/%.glb
	@mkdir -p $(dir $@)
	@echo "    [MODEL] $@"
	@$(N64_MKMODEL) -o "$(dir $@)" "$<"

$(BUILD_DIR)/skinning-test.dfs: $(ASSETS)
$(BUILD_DIR)/skinning-test.elf: $(OBJECTS)

skinning-test.z64: N64_ROM_TITLE = "Skinning test"
skinning-test.z64: $(BUILD_DIR)/skinning-test.dfs

clean:
	rm -rf $(BUILD_DIR) $(FILESYSTEM_DIR) *.z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean