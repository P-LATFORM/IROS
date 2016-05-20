################################################################################
#
# @file unittest.mk
#
# @author Murat Cakmak
#
# @brief Makefile to build and run an unit test
#			> Builds and Runs Unit test
#			> Runs Code Coverage
#			> Runs Static Analysis Tools
#			> TODO Runs Sanity Checks
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

################################################################################
#                    		DEFINITIONS & INCLUDES                             #
################################################################################

# Path of P-OS Root
ROOT_PATH = .

#
# Get Environment Info for Unittest Build
#
ENV ?= x86
ENV_MAKE_FILE = Environment/Target/$(ENV)/environment.mk
ifeq ($(wildcard $(ENV_MAKE_FILE)),)
$(error Invalid Environment : $(ENV))
endif

# include environment
include $(ENV_MAKE_FILE)

include $(TEST_MODULE)/module.mk
#
# Include Specified Unit Test 
#
TEST_DIR = $(TEST_MODULE)/UnitTest
include $(TEST_DIR)/unittest.mk

# Path of Unity Tool
UNITY_ROOT = $(ROOT_PATH)/Environment/Tools/Unity

# Path of out files after Unity Build
UNITY_OUT_PATH = $(ROOT_PATH)/out/UnitTest/$(TEST_TARGET_NAME)

#
# Test file name
#	IMP : Unit test file must be name as unittest_<TARGET>
#
TEST_FILE_NAME = $(TEST_TARGET_NAME)_UnitTest
# Test source file
TEST_FILE = $(TEST_FILE_NAME).c
# Test runner file
TEST_RUNNER_FILE = $(UNITY_OUT_PATH)/$(TEST_FILE_NAME)_runner.c

#
# Test output (executable) file
#
TARGET = $(UNITY_OUT_PATH)/$(TEST_TARGET_NAME)$(UNITTEST_TARGET_EXTENSION)

#
# All source file for unit test
#	- Unity.c : Source code of Unity tool
#	- Unit Test source file
#	- Test Runner source file
#
ALL_SRC_FILES= \
	$(UNITY_ROOT)/unity.c \
	$(TEST_DIR)/$(TEST_FILE) \
	$(TEST_RUNNER_FILE)

#
# Include Directories
#	- Header files of Unity Tool
#	- Project Common path for common type definitions 
#
INC_DIRS = \
	-I$(UNITY_ROOT) \
	-I$(ROOT_PATH)/Include \
	-I$(ROOT_PATH)/Include/BSP \
	-I$(ROOT_PATH)/Kernel \
	-I$(TEST_DIR) \
	-I$(TEST_DIR)/Mock

#
# CFLAGS
#	- Code Coverity Flags for GCOV Tool
#	- Environment specific Unit test flags (UNITTEST_CFLAGS)  
#
CFLAGS += \
	-ftest-coverage -fprofile-arcs -g -O0 --coverage \
	$(UNITTEST_CFLAGS)

# 
# Compiler Symbols
#
SYMBOLS += -DTEST

################################################################################
#                    		     RULES                                   	   #
################################################################################

#
# Default Rule
#	- Runs Unit Test (Unity)
#	- Runs Code Coverity (GCOV)
#
default:
# Create directory for Test out
	mkdir -p $(UNITY_OUT_PATH)
	@echo "\n\n***************************************************************"
	@echo "         MODULE CHECK FOR $(TEST_MODULE)"
	@echo "***************************************************************"
	@echo "\n===================== UNIT TESTS =============================="

# Unit Test source needs unity.h file but relative location of unity.h can
# be different for each module (unit) and unit test developer needs to
# include unity.h with its relative path in unit test source file. 
# To avoid that, we copy the unity.h into Unit Test directory temproralily 
# In this way, developer just include without relative path. 
# When compiling is finished, we remove unity.h
	cp $(UNITY_ROOT)/unity.h $(TEST_DIR)
		
#
# Unity requires Test Runner Module and following RUBY script extracts test 
# runner modules automatically from Unit Test file
#
	ruby \
		$(UNITY_ROOT)/scripts/generate_test_runner.rb \
		$(TEST_DIR)/$(TEST_FILE) \
		$(TEST_RUNNER_FILE)
	
#
# Compile all unit test
#
	$(CC) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(ALL_SRC_FILES) -o $(TARGET)
	
# After compiling, we do not need unity.h in Unit Test directory, remove it.
	rm $(TEST_DIR)/unity.h
	
#
# Run Unit Test and see results
#
	./$(TARGET)

	@echo "\n===================== CODE COVERAGE =============================="
	
# GCOV tool requires unit test code for code coverage analysis, to work on
# single directory, we also copy unit test source file into unity out directory
	cp $(TEST_DIR)/$(TEST_FILE) $(UNITY_OUT_PATH)

# GCOV puts coverage files to makefile (this file) location but all related
# files should be in same directory to make GCOV execution more simple so
# move gcda and gcno file to Unity Out path which already
	mv *.gcda *.gcno  $(UNITY_OUT_PATH)

# Run GCOV tool to see code coverage
	gcov $(UNITY_OUT_PATH)/$(TEST_FILE) -n

	@echo "===================== CODE ANALYSIS =============================="
		
	splint $(TEST_MODULE)/$(MODULE_SRC_FILES) $(INC_DIRS)
