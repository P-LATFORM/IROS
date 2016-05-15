################################################################################
#
# @file build_projects.mk
#
# @author Murat Cakmak
#
# @brief Makefile to build a specific project
#
#	Build system can be managed through this file.
#	Requires a valid project path and project.mk 
#	
# @see http://https://github.com/ir-os/IROS
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

# Even if this file is located under different directory, it is called from
# main makefile which under main directory so need to select main directory 
# as ROOT path
ROOT_PATH = .

################################################################################
#                    		   PARAMETER CHECKS                            	   #
################################################################################

# PROJECT argument is a must
ifndef PROJECT
$(error Please specify a project. (Example make PROJECT=<Projects_Name> ) )
endif

# Project specific make file. 
PROJECT_MAKE_FILE = $(ROOT_PATH)/Projects/$(PROJECT)/project.mk

# Check file existency. 
ifeq ($(wildcard $(PROJECT_MAKE_FILE)),)
$(error Invalid Project! Project.mk does not exist in "Projects" folders : $(PROJECT_MAKE_FILE))
endif 

# Include project make file and fetch project specific configurations.
include $(PROJECT_MAKE_FILE)

################################################################################
#                    		   	DEFINES                            	   		   #
################################################################################

#
# Path of Selected CPU
# 
CPU_PATH = $(ROOT_PATH)/HAL/CPU/$(CPU)

#
# Path of Selected Board
#  
BOARD_PATH = $(ROOT_PATH)/HAL/Board/$(BOARD)

##
# Path of OS Files
# 
OS_PATH = $(ROOT_PATH)/OS

#
# Compiler Flags
#
CFLAGS += \
	-I$(CPU_PATH) \
	-I$(BOARD_PATH) \
	-ICommon
#$(INC_PATHS) \

################################################################################
#                    		   	RULES                            	   		   #
################################################################################

$(PROJECT):
	@echo $(PROJECT) will be compiling later...

#      : $(SRC_FILES)
#	@echo $(SRC_FILES)
#	$(CCD) -o $(PROJECT_NAME) $(SRC_FILES) $(CFLAGS)
#	@echo "\n--------------------------------------\n"
#	./$(PROJECT_NAME)


