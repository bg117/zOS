# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

EXEC     := zos-dev-build.img
FULLEXEC := ${BUILD_DIR}/${EXEC}

BUILD_TYPE ?= debug
CMP_BUILD_TYPE := ${shell echo ${BUILD_TYPE} | tr '[:upper:]' '[:lower:]'}

ifeq (${CMP_BUILD_TYPE},debug)
COMFLAGS := -O0 \
			-g
else ifeq (${CMP_BUILD_TYPE},release)
COMFLAGS := -O1 \
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

export SRC_DIR=${CURDIR}
export CFLAGS=${COMFLAGS}
export CXXFLAGS=${COMFLAGS}

.PHONY: all build boot utils kernel modules clean always

all: build

build: boot utils kernel modules ${FULLEXEC}

boot:
	${MAKE} -C ${CURDIR}/boot

utils:
	${MAKE} -C ${CURDIR}/utils

kernel:
	${MAKE} -C ${CURDIR}/kernel

modules:
	${MAKE} -C ${CURDIR}/modules ${MODULES_TARGET}

clean:
	${MAKE} -C ${CURDIR}/boot clean
	${MAKE} -C ${CURDIR}/kernel clean

always:

${FULLEXEC}: always
	dd if=/dev/zero of=$@ bs=66060288 count=1
	mformat -t 1024 -h 2 -s 63 -R 4 -i $@ ::

	dd if=${BIN_DIR}/osldr of=$@ conv=notrunc

	mmd -i ${FULLEXEC} ::SYS

	mcopy -i ${FULLEXEC} ${BIN_DIR}/zs ::SYS/

	mcopy -i ${FULLEXEC} ${BIN_DIR}/modules/ ::SYS/
