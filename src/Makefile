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
	dd if=/dev/zero of=$@ bs=1474560 count=1
	mkfs.vfat -F12 $@

	dd if=${BIN_DIR}/ldr.z of=$@ count=1 conv=notrunc
	dd if=${ROOT_DIR}/text.bios of=$@ count=1 seek=1 conv=notrunc
