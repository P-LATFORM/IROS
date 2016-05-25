CPU_SRC_FILES := $(CPU_PATH)/internal/startup_ARMCM3.s $(shell /usr/bin/find $(CPU_PATH) -mindepth 0 -maxdepth 3 -name "*.c" ! -path "*UnitTest*")

MODULE_INC_PATHS += \
	-I$(CPU_PATH)/internal
