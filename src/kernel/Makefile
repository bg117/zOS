# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

EXEC     := zs
FULLEXEC := ${BIN_DIR}/${EXEC}

ASM      := nasm
ASMFLAGS := -f elf

CC      := i686-elf-gcc
LD      := i686-elf-ld
OBJCOPY := i686-elf-objcopy

TARGET := i686-pc-none-elf

LIBGCCDIR := ${shell dirname `${CC} -print-libgcc-file-name`}

CFLAGS       := -Wall \
				-Og \
				-g \
				-fno-stack-check \
				-ffreestanding \
				-nostdlib

CPPFLAGS     := -I${CURDIR}/c/include \
				-Wno-unknown-pragmas

LDFLAGS      := -Map=${OBJ_DIR}/${EXEC}.map \
				-T${CURDIR}/linker.ld \
				-L${LIBGCCDIR} \
				-nostdlib

OBJCOPYFLAGS := -O binary \
				--strip-unneeded

SRCS_ASM := ${wildcard ${CURDIR}/asm/*.asm}
SRCS_C   := ${wildcard ${CURDIR}/c/*.c}

OBJS_ASM := ${subst ${CURDIR}/asm,${OBJ_DIR},${SRCS_ASM:.asm=.o}}
OBJS_C   := ${subst ${CURDIR}/c,${OBJ_DIR},${SRCS_C:.c=.o}}

.PHONY: all build clean

all: build

build: ${FULLEXEC}

clean:
	${RM} ${OBJS_ASM} ${OBJS_C}

${FULLEXEC}: ${FULLEXEC}.elf
	${OBJCOPY} \
		${OBJCOPYFLAGS} \
		$< \
		$@

${FULLEXEC}.elf: ${OBJS_ASM} ${OBJS_C}
	${LD} \
		${LDFLAGS} \
		-o $@ \
		$^ \
		-lgcc

${OBJ_DIR}/%.o: ${CURDIR}/c/%.c
	${CC} \
		${CFLAGS} \
		${CPPFLAGS} \
		-c \
		-o $@ \
		$<

${OBJ_DIR}/%.o: ${CURDIR}/asm/%.asm
	${ASM} \
		${ASMFLAGS} \
		-o $@ \
		$<
