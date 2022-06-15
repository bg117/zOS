ASM      := nasm
ASMFLAGS := -f bin

SRCS := ${CURDIR}/boot.asm
INCS := ${wildcard ${CURDIR}/*.inc}

EXEC     := zboot
FULLEXEC := ${BIN_DIR}/${EXEC}

.PHONY: all build clean

all: build

build: ${FULLEXEC}

clean:
	${RM} ${FULLEXEC}

${FULLEXEC}: ${SRCS} ${INCS}
	${ASM} ${ASMFLAGS} -o $@ $<
