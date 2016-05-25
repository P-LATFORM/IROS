################################################################################
#
# @file module.mk
#
# @author Murat Cakmak
#
# @brief Module make file
#
# @see https://github.com/P-LATFORM/P-OS/wiki
#
#*****************************************************************************
#
# The MIT License (MIT)
#
# Copyright (c) 2016 P-OS
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
################################################################################

#
# Get all source files (.c files) using 'find' command except UnitTest folder
#
CPU_SRC_FILES := $(CPU_PATH)/internal/startup_ARMCM3.s $(shell /usr/bin/find $(CPU_PATH) -mindepth 0 -maxdepth 3 -name "*.c" ! -path "*UnitTest*")

MODULE_INC_PATHS += \
	-I$(CPU_PATH)/internal
