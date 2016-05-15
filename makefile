################################################################################
#
# @file makefile .c
#
# @author Murat Cakmak
#
# @brief Main makefile of build system
#
#	Build system can be managed through this file. 
#	
#		- Build a Project
#			[USAGE] : 
#				make PROJECT=<PROJECT_NAME>
#		
#			Builds a project. Uses project.mk file under project 
#			directory to get project configurations. 
#
#		- Run a Unit test
#			[USAGE] : 
#				make unittest TEST_MODULE=<UNIT_NAME>
#		
#			Builds and runs a Unit test. Uses unittest.mk file under unittest 
#			directory to get test configurations. 
#
#			 	> Runs and prints Unit Test Resuts (PASS/FAIL)
#			 	> Runs and prints Code Coverage Results (% of coverage)
#
#		- Check All System Stability
#			[USAGE] : 
#				make check_all
#
#			Tests and checks all system.
#				> Builds all projects under Projects directory. 
#				> Runs all unittests, code coverage, static analysis
#				> Runs all Integration Tests
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

################################################################################
#                    		  DEFINITIONS                                      #
################################################################################

ENV ?= x86

#
# Root Path 
#
ROOT_PATH = .

# Path which includes all makefiles
MAKE_FILES_PATH = Utilities/Makefiles

#
# Detailed Compiling output
#
ifndef V
SILENCE = -s
else
SILENCE =
endif

################################################################################
#                    		     RULES                                   	   #
################################################################################

#
# Default Rule. Builds a Projects
#
default:
	@echo "Projects is compiled..."

#
# Builds and Runs a Unit Test
#
unittest:
	@echo "Unit is tested..."

#
# Builds and Runs all system validation objects. 
# Detailed General System Check
#
check_all: clean
	@echo "All system is checked..."

#
# Cleans build output 
#
clean:
	@echo "Cleaned..."
	rm -rf out
