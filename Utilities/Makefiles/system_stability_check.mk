################################################################################
#
# @file system_stability_check.mk
#
# @author Murat Cakmak
#
# @brief Checks All Systems Stability
#			
#			- TODO : Builds all projects
#			- TODO : Runs all unit tests, unit analysis and sanity checks 
#			- TODO : Runs all integration 
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

# Path which includes all makefiles
MAKE_FILES_PATH = Utilities/Makefiles
# 
# TODO
# We can get all unit test files by following execution but 'make' command does
# not work consequently in a loop for different parameters, so let's break
# unit tests with just first one
# 
UNIT_TEST_FILES := $(shell find HAL/CPU -mindepth 1 -maxdepth 6 -name "unittest.mk")

default:
# Visit all unit test and run
	$(foreach tst_module, $(UNIT_TEST_FILES), \
		make -f $(MAKE_FILES_PATH)/unittest.mk TEST_MODULE=$(subst /unittest/unittest.mk,,$(tst_module)) \
	)
