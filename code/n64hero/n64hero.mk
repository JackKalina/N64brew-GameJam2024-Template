ASSET_DIR := n64hero

# Add .t3dm and .sprite files dynamically from assets/$(ASSET_DIR)
ASSETS_LIST += $(shell find assets/$(ASSET_DIR) -name "*.glb" -exec bash -c 'echo filesystem/$(ASSET_DIR)/$$(basename "{}" .glb).t3dm' \;)
ASSETS_LIST += $(shell find assets/$(ASSET_DIR) -name "*.png" -exec bash -c 'echo filesystem/$(ASSET_DIR)/$$(basename "{}" .png).sprite' \;)

# Custom flags for specific assets
filesystem/$(ASSET_DIR)/m6x11plus.font64: MKFONT_FLAGS += --outline 1 --size 36