################################################################################
#
# @file environment.mk
#
# @author Murat Cakmak
#
# @brief Configurations and definitions for x86 Environment
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
#							COMPILING & LINKING
################################################################################
#
# C Compiler for x86 : GCC
#
CC = gcc

################################################################################
#								TESTING
################################################################################

#
# Unit test extensions for Unit test runner applications
#
ifeq ($(OS), Windows_NT)
	UNITTEST_TARGET_EXTENSION = .exe
else
	UNITTEST_TARGET_EXTENSION = .out
endif

#
# Unit test specific CFLAGS for x86 platform
#
UNITTEST_CFLAGS = -std=c99 -Wall -Wextra -Werror  -Wpointer-arith -Wcast-align -Wwrite-strings \
            -Wswitch-default -Wunreachable-code -Winit-self -Wmissing-field-initializers \
            -Wno-unknown-pragmas -Wstrict-prototypes -Wundef -Wold-style-definition