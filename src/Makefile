# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

EXEC     := zos-dev-build.img
FULLEXEC := ${BUILD_DIR}/${EXEC}

.PHONY: all build boot kernel clean always

all: build

build: boot kernel ${FULLEXEC}

boot:
	${MAKE} -C ${CURDIR}/boot

kernel:
	${MAKE} -C ${CURDIR}/kernel

clean:
	${MAKE} -C ${CURDIR}/boot clean
	${MAKE} -C ${CURDIR}/kernel clean

always:

${FULLEXEC}: always
	dd if=/dev/zero of=$@ bs=66060288 count=1
	mformat -t 1024 -h 2 -s 63 -R 4 -i $@ ::

	dd if=${BIN_DIR}/ldr.z of=$@ conv=notrunc

	mcopy -i ${FULLEXEC} ${BIN_DIR}/zs ::
