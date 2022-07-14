# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

ifndef VERBOSE
MAKEFLAGS += --no-print-directory
endif

BUILD_TYPE ?= debug
CMP_BUILD_TYPE := ${shell echo ${BUILD_TYPE} | tr '[:upper:]' '[:lower:]'}

COMFLAGS := -Wall \
			-Wextra \
			-Wpedantic \
			-Wshadow \
			-Wcast-qual \
			-Wpointer-arith \
			-Wstrict-prototypes

ifeq (${CMP_BUILD_TYPE},debug)
COMFLAGS += -O0 \
			-g
else ifeq (${CMP_BUILD_TYPE},release)
COMFLAGS += -O1 \
			-fcaller-saves \
			-fcode-hoisting \
			-fcrossjumping \
			-fcse-follow-jumps \
			-fcse-skip-blocks \
			-fdelete-null-pointer-checks \
			-fdevirtualize \
			-fdevirtualize-speculatively \
			-ffinite-loops \
			-fgcse \
			-fgcse-lm  \
			-fhoist-adjacent-loads \
			-fipa-bit-cp \
			-fipa-cp \
			-fipa-icf \
			-fipa-ra \
			-fipa-sra \
			-fipa-vrp \
			-fisolate-erroneous-paths-dereference \
			-flra-remat \
			-finline-functions \
			-finline-small-functions \
			-findirect-inlining
else
${error Unknown build type: ${BUILD_TYPE}}
endif

export TARGET=i686-elf-

export CC=${TARGET}gcc
export CXX=${TARGET}g++
export AS=nasm
export AR=${TARGET}ar
export LD=${TARGET}ld
export OBJCOPY=${TARGET}objcopy

export ROOT_DIR=${CURDIR}

export BUILD_DIR=${CURDIR}/build

export BIN_DIR=${BUILD_DIR}/bin
export LIB_DIR=${BUILD_DIR}/lib
export OBJ_DIR=${BUILD_DIR}/obj

export SYSROOT=${BUILD_DIR}/root

export CFLAGS=${COMFLAGS}
export CXXFLAGS=${COMFLAGS}

.PHONY: all build clean check-dirs

all: check-dirs build

build:
	@echo Building sources...

	@${MAKE} -C ${CURDIR}/src

	@echo Done. Sysroot at: ${SYSROOT}

clean:
	@echo Cleaning build directory...

	@${RM} -r ${BUILD_DIR}

check-dirs:
	@echo Checking if build directories exist...

	@-mkdir -p ${BIN_DIR}
	@-mkdir -p ${OBJ_DIR}
	@-mkdir -p ${LIB_DIR}
	@-mkdir -p ${SYSROOT}
