# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

EXEC     := zos-dev-build.img
FULLEXEC := ${BUILD_DIR}/${EXEC}

export SRC_DIR=${CURDIR}

.PHONY: all help build boot utils kernel modules sysroot clean always

all: build

build: boot utils kernel modules sysroot ${FULLEXEC}

boot:
	@${MAKE} -C ${CURDIR}/boot

utils:
	@${MAKE} -C ${CURDIR}/utils

kernel:
	@${MAKE} -C ${CURDIR}/kernel

modules:
	@${MAKE} -C ${CURDIR}/modules ${MODULES_TARGET}

sysroot:
	@echo Creating sysroot...

	@-mkdir -p ${SYSROOT}/sys
	@cp -a ${BIN_DIR}/zs ${SYSROOT}/sys/

	@cp -a ${RESOURCES_DIR}/ ${SYSROOT}/sys

clean:
	@echo Cleaning up sources...

	@${MAKE} -C ${CURDIR}/boot clean
	@${MAKE} -C ${CURDIR}/kernel clean
	@${RM} -r ${SYSROOT}

always:

${FULLEXEC}: always
	@echo Creating hard disk image: ${EXEC}...

	@dd if=/dev/zero of=$@ bs=66060288 count=1
	@mformat -t 1024 -h 2 -s 63 -R 4 -i $@ ::

	@dd if=${BIN_DIR}/osldr of=$@ conv=notrunc

	@mcopy -sp -i ${FULLEXEC} ${SYSROOT}/* ::
