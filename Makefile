# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

ifndef VERBOSE
MAKEFLAGS += --no-print-directory
endif

BUILD_TYPE    ?= debug
SERIAL_STDOUT ?=
SERIAL_OUTPUT ?=

CMP_BUILD_TYPE    := ${shell echo ${BUILD_TYPE} | tr '[:upper:]' '[:lower:]'}
CMP_SERIAL_STDOUT := ${shell echo ${SERIAL_STDOUT} | tr '[:upper:]' '[:lower:]'}

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
	COMFLAGS += -O2 \
				-fno-align-functions \
				-fno-align-jumps \
				-fno-align-labels \
				-fno-align-loops \
				-fno-prefetch-loop-arrays \
				-fno-expensive-optimizations
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

QEMUFLAGS := -drive file=${BUILD_DIR}/zos-dev-build.img,format=raw

ifeq (${CMP_SERIAL_STDOUT},yes)
	QEMUFLAGS += -serial stdio
else ifeq (${CMP_SERIAL_STDOUT},${filter ${CMP_SERIAL_STDOUT},no })
	ifneq (${SERIAL_OUTPUT},)
		QEMUFLAGS += -serial file:${SERIAL_OUTPUT}
	endif
endif

ifeq (${OS},Windows_NT)
	BOCHSRC := bochsrc_win32.bxrc
else
	BOCHSRC := bochsrc_unix.bxrc
endif

ALL_TARGETS := all help build clean run-qemu run-bochs check-dirs

.PHONY: ${ALL_TARGETS}

all: check-dirs build

help:
	@echo Targets:
	@echo ${ALL_TARGETS}

build:
	@echo Building sources...

	@${MAKE} -C ${CURDIR}/src

	@echo Done. Sysroot at: ${SYSROOT}

clean:
	@echo Cleaning build directory...

	@${RM} -r ${BUILD_DIR}

run-qemu:
	@echo Running zOS on QEMU...
	@qemu-system-i386 \
		${QEMUFLAGS}

run-bochs:
	@echo Running zOS on Bochs...
	@bochs \
		-q \
		-f ${BOCHSRC}

check-dirs:
	@echo Checking if build directories exist...

	@-mkdir -p ${BIN_DIR}
	@-mkdir -p ${OBJ_DIR}
	@-mkdir -p ${LIB_DIR}
	@-mkdir -p ${SYSROOT}
