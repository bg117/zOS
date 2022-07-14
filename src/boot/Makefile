# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

ASFLAGS := -f bin

SRCS := ${CURDIR}/boot.asm
INCS := ${wildcard ${CURDIR}/*.inc}

EXEC     := osldr
FULLEXEC := ${BIN_DIR}/${EXEC}

.PHONY: all build clean

all: build

build:
	@echo Building bootloader...
	@${MAKE} ${FULLEXEC}

clean:
	@echo Cleaning up bootloader binary...
	@${RM} ${FULLEXEC}

${FULLEXEC}: ${SRCS} ${INCS}
	@echo Assembling bootloader...
	@${AS} \
		${ASFLAGS} \
		-o $@ \
		$<
