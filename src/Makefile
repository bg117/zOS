# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

EXEC     := zos-dev-build.img
FULLEXEC := ${BUILD_DIR}/${EXEC}

export SRC_DIR=${CURDIR}

.PHONY: all build boot kernel modules clean always

all: build

build: boot kernel modules ${FULLEXEC}

boot:
	${MAKE} -C ${CURDIR}/boot

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
