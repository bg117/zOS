# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

ASM      := nasm
ASMFLAGS := -f bin

SRCS := ${CURDIR}/boot.asm
INCS := ${wildcard ${CURDIR}/*.inc}

EXEC     := osldr
FULLEXEC := ${BIN_DIR}/${EXEC}

.PHONY: all build clean

all: build

build: ${FULLEXEC}

clean:
	${RM} ${FULLEXEC}

${FULLEXEC}: ${SRCS} ${INCS}
	${ASM} \
		${ASMFLAGS} \
		-o $@ \
		$<
