# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

EXEC     := zs
FULLEXEC := ${BIN_DIR}/${EXEC}

ASM      := nasm
ASMFLAGS := -f elf32

CC      := ${TARGET}gcc
LD      := ${TARGET}ld
OBJCOPY := ${TARGET}objcopy

LIBGCCDIR := ${shell dirname `${CC} -print-libgcc-file-name`}

CFLAGS += -Wall \
		  -Wextra \
		  -fno-stack-check \
		  -ffreestanding \
		  -nostdlib

CPPFLAGS := -I ${SRC_DIR}/include/common \
			-MMD

LDFLAGS := -Map ${OBJ_DIR}/${EXEC}.map \
		   -T ${CURDIR}/linker.ld \
		   -L ${LIBGCCDIR} \
		   -L ${LIB_DIR} \
		   -nostdlib

OBJCOPYFLAGS := -O binary \
				--strip-unneeded

SRCS_ASM := ${wildcard ${CURDIR}/asm/*.asm}
SRCS_C   := ${wildcard ${CURDIR}/c/*.c}
SRCS_H   := ${wildcard ${SRC_DIR}/include/common/kernel/*.h}

OBJS_ASM := ${subst ${CURDIR}/asm,${OBJ_DIR},${SRCS_ASM:.asm=.o}}
OBJS_C   := ${subst ${CURDIR}/c,${OBJ_DIR},${SRCS_C:.c=.o}}

HDRS := ${OBJS_C:.o=.d}

.PHONY: all build clean format

all: format build

format: ${SRCS_C} ${SRCS_H}
	clang-format -i $^

build: ${FULLEXEC}

clean:
	${RM} ${OBJS_ASM} ${OBJS_C} ${HDRS} ${FULLEXEC} ${FULLEXEC}.r

${FULLEXEC}: ${FULLEXEC}.r
	${OBJCOPY} \
		${OBJCOPYFLAGS} \
		$< \
		$@

${FULLEXEC}.r: ${OBJS_ASM} ${OBJS_C}
	${LD} \
		${LDFLAGS} \
		-o $@ \
		$^ \
		-lutils \
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

-include ${HDRS}
