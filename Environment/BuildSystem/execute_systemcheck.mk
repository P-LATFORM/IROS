################################################################################
#
# @file system_stability_check.mk
#
# @author Murat Cakmak
#
# @brief Checks All Systems Stability
#			
#			- TODO : Builds all projects
#			- Runs all unit tests, unit analysis and sanity checks 
#			- TODO : Runs all integration 
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

# Path which includes all makefiles
MAKE_FILES_PATH = Environment/BuildSystem

#
# Get all projects
#
PROJECT_PATHS := $(wildcard ./Projects/*)

# 
# Get all unit tests
# 
UNIT_TEST_FILES := $(shell /usr/bin/find . -mindepth 1 -maxdepth 6 -name "unittest.mk")


################################################################################
#                    		     RULES                                   	   #
################################################################################

#
# Default rule
#
#	- Builds all projects
#	- Builds and run all unittests
#	- TODO Build and run all integration test
#
default: build_projects run_unittests run_integrationtests

#
# Rule to build all projects under Project Directory.
#
build_projects: $(PROJECT_PATHS)
$(PROJECT_PATHS):
# We already have a makefile which builds Projects so let's just call make file with required argument (to be built project name path)
	$(MAKE) -f $(MAKE_FILES_PATH)/build_project.mk PROJECT=$(lastword $(subst /, ,$@))
.PHONY: $(PROJECT_PATHS)

#
# Rule to build and run all Unit Tests
#
run_unittests: $(UNIT_TEST_FILES)
$(UNIT_TEST_FILES):
# We already have a makefile which runs Unittest so let's just call make file with required argument (to be tested module path)
	$(MAKE) -f $(MAKE_FILES_PATH)/execute_unittest.mk TEST_MODULE=$(subst /UnitTest/unittest.mk,,$@)
.PHONY: $(UNIT_TEST_FILES)

#
# Rule to run all integration test
#
run_integrationtests:
	@echo "\n >>> UPS! Integration_Tests not ready yet"
