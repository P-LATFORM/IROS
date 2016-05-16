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
# Get all unit tests
# 
UNIT_TEST_FILES := $(shell find . -mindepth 1 -maxdepth 6 -name "unittest.mk")

#
# Search and find all Unit Tests parallely. 
#
run_unittests: $(UNIT_TEST_FILES)
$(UNIT_TEST_FILES):
# We already have a makefile which runs Unittest so let's just call make file with required argument (to be tested module path)
	$(MAKE) -f $(MAKE_FILES_PATH)/execute_unittest.mk TEST_MODULE=$(subst /unittest/unittest.mk,,$@)
.PHONY: $(UNIT_TEST_FILES)

#
# Runs all integration tests
#
run_integrationtests:
	@echo "Integration_Tests is not ready yet"

#
# Builds all projects to check compliling & linking errors.
#
build_projects:
	@echo "Project_Build is not ready yet"

#
# Default rule to run all tests and checks
#
default: run_unittests run_integrationtests build_projects
