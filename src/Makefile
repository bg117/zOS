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
	mformat -t 80 -h 2 -s 18 -R 5 -i $@ ::

	dd if=${BIN_DIR}/ldr.z of=$@ conv=notrunc

	mcopy -i ${FULLEXEC} ${BIN_DIR}/zs ::
