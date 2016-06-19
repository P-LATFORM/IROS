################################################################################
#
# @file build_projects.mk
#
# @author Murat Cakmak
#
# @brief Builds a project
#
# @see https://github.com/P-LATFORM/P-OS/wiki
#
#*****************************************************************************
#
#  Copyright (2016), P-OS
#
#   This software may be modified and distributed under the terms of the
#   'MIT License'.
#
#   See the LICENSE file for details.
#
################################################################################

# Path of P-OS Root
ROOT_PATH = .

# Defaut Environment
ENV ?= ARM

################################################################################
#                    			BASIC CHECKS
################################################################################

# PROJECT must be defined.
ifndef PROJECT
$(error Please select a project. (Example make PROJECT=<Projects_Name> ) )
endif

# Get project path and its makefile
PROJECT_PATH = $(ROOT_PATH)/Projects/$(PROJECT)
PROJECT_MAKE_FILE = $(PROJECT_PATH)/project.mk

ifeq ($(wildcard $(PROJECT_MAKE_FILE)),)
$(error Invalid Project! Project does not exist in "Projects" folders : $(PROJECT_MAKE_FILE))
endif 

#
# Get Environment Info for Projects
#
# Default is ARM
ENV_PATH = Environment/Target/$(ENV)
ENV_MAKE_FILE = $(ENV_PATH)/environment.mk
ifeq ($(wildcard $(ENV_MAKE_FILE)),)
$(error Invalid Environment : $(ENV))
endif

################################################################################
#                    		DEFINITIONS & INCLUDES                             #
################################################################################

# Include Project
include $(PROJECT_MAKE_FILE)

# Include Environment
include $(ENV_MAKE_FILE)

# Path of Selected CPU
CPU_PATH = $(ROOT_PATH)/BSP/CPU/$(CPU)

# Path of Selected Board
BOARD_PATH = $(ROOT_PATH)/BSP/Board/$(BOARD)

# Path of OS Files
KERNEL_PATH = $(ROOT_PATH)/Kernel

# Path of Helper Tools e.g Debug
TOOLS_PATH = $(ROOT_PATH)/Environment/Tools

# Directory for Project Output Files (.o, bin, elf etc.)
PROJECT_OUT_PATH = $(ROOT_PATH)/out/Projects/$(PROJECT)
PROJECT_OBJS_OUT_PATH= $(PROJECT_OUT_PATH)/obj


# Get all Kernel Source files except Unit Test files
KERNEL_SRC_FILES := $(shell /usr/bin/find $(KERNEL_PATH) -mindepth 1 -maxdepth 6 -name "*.c" ! -path "*UnitTest*")
# Get all Project (Application) Source files except Unit Test files
PROJECT_SRC_FILES := $(shell /usr/bin/find $(PROJECT_PATH) -mindepth 1 -maxdepth 6 -name "*.c" ! -path "*PSoCCreator*")

# Include CPU, Board and Kernel makefiles to get specific rules
include $(CPU_PATH)/module.mk
include $(BOARD_PATH)/module.mk
include $(KERNEL_PATH)/Kernel.mk

# Collect all source files
SRC_FILES = \
	$(KERNEL_SRC_FILES) \
	$(CPU_SRC_FILES) \
	$(BOARD_SRC_FILES) \
	$(PROJECT_SRC_FILES)

# Collect all Inclıde Files
INCLUDE_PATHS = \
	$(PROJECT_INC_LIB) \
	$(SYS_INC_PATHS) \
	$(PROJECT_INC_PATHS) \
	$(KERNEL_INC_PATHS) \
	$(MODULE_INC_PATHS) \
	-I$(ROOT_PATH)/Include \
	-I$(ROOT_PATH)/Include/BSP \
	-I$(PROJECT_PATH)/Config \
	-I$(TOOLS_PATH)/Debug

# Target File name including all path
BUILD_TARGET = $(PROJECT_OUT_PATH)/$(PROJECT)

# Building depends on object (.o) files so let's get object versions of source files
PROJECT_OBJECTS := $(SRC_FILES) # Get all source files
PROJECT_OBJECTS := $(PROJECT_OBJECTS:.c=.o) # Convert .c extensions to .o
PROJECT_OBJECTS := $(PROJECT_OBJECTS:.s=.o) # Convert .s extensions to .o

#
# Compiler specific Flags
#
LIBRARY_PATHS =  $(PROJECT_INC_LIB) $(PROJECT_LIB_LIB) $(SYS_LIB_PATHS)
CC_FLAGS = $(MCU_CC_FLAGS) -c $(CC_OPTIM_FLAGS) $(CC_DEBUG_FLAGS) -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections 
LD_FLAGS = $(MCU_CC_FLAGS) -Wl,--gc-sections $(SYS_LD_FLAGS) -Wl,-Map=$(BUILD_TARGET).map 
LD_SYS_LIBS = $(SYS_LIBRARIES)

################################################################################
#                    		     RULES                                   	   #
################################################################################

#
# Default rule to build a project
#
#	Dependent to
#  - Out folder creation
#  - Project HEX (.bin) file creation
#
default: createoutdir $(BUILD_TARGET).bin
	@echo "\nProject '$(PROJECT)' Compiled...\n"

#
# Rule to build Assembly (.s) files
#
.s.o:
	$(AS) $(MCU_CC_FLAGS) -o $@ $<

#
# Rule to build C (.c) files
#
.c.o:
	@echo "Compile " $<
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) $(INCLUDE_PATHS) -o $@  $<

#
# Rule to build C++ (.cpp) files
#
.cpp.o:
	$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu++98 $(INCLUDE_PATHS) -o $@ $<

#
# Rule to create .ELF file
#
# 	Dependent to
#	- Object File Creations
#	- System Objects
#
$(BUILD_TARGET).elf: $(PROJECT_OBJECTS) $(SYS_OBJECTS)
	$(LD) $(LD_FLAGS) -T$(LINKER_SCRIPT) $(LIBRARY_PATHS) -o $@ $^ $(PROJECT_LIBRARIES) $(SYS_LIBRARIES) $(PROJECT_LIBRARIES) $(SYS_LIBRARIES)

# TODO FIX it. Hacky code. Normally, we need to create object files under out dir but I (Murat Çakmak) could not.
# As a temprory solution, create object file under source fine directories and when finished building, collect(move) them under out folder
	mkdir -p $(PROJECT_OBJS_OUT_PATH)
	mv $(PROJECT_OBJECTS) $(PROJECT_OBJS_OUT_PATH)

#
# Rule to create Project HEX (.bin) file
#
#	Dependent to ELF file creation
#
$(BUILD_TARGET).bin: $(BUILD_TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Rule to clean Project
clean:
	rm -rf $(PROJECT_OUT_PATH)

# Creates output path
createoutdir:
	mkdir -p $(PROJECT_OUT_PATH)
