MODULE_SRC_FILES += Board.c

BOARD_SRC_FILES := $(shell /usr/bin/find $(BOARD_PATH) -mindepth 0 -maxdepth 6 -name "*.c" ! -path "*UnitTest*")