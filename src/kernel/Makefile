# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

EXEC     := zs
FULLEXEC := ${BIN_DIR}/${EXEC}

ASM      := nasm
ASMFLAGS := -f elf32

BUILD_TYPE ?= debug

CC      := i686-elf-gcc
LD      := i686-elf-ld
OBJCOPY := i686-elf-objcopy

LIBGCCDIR := ${shell dirname `${CC} -print-libgcc-file-name`}

CFLAGS := -Wall \
		  -Wextra \
		  -fno-stack-check \
		  -ffreestanding \
		  -nostdlib

CPPFLAGS := -I ${SRC_DIR}/include/common \
			-MMD

LDFLAGS := -Map ${OBJ_DIR}/${EXEC}.map \
		   -T ${CURDIR}/linker.ld \
		   -L ${LIBGCCDIR} \
		   -nostdlib

ifeq (${BUILD_TYPE},debug)
CFLAGS += -O0 \
		  -g
else ifeq (${BUILD_TYPE},release)
# manually turn on optimizations that DON'T break the code
CFLAGS += -O1 \
		  -fcaller-saves \
		  -fcode-hoisting \
		  -fcrossjumping \
		  -fcse-follow-jumps \
		  -fcse-skip-blocks \
		  -fdelete-null-pointer-checks \
		  -fdevirtualize \
		  -fdevirtualize-speculatively \
		  -ffinite-loops \
		  -fgcse \
		  -fgcse-lm  \
		  -fhoist-adjacent-loads \
		  -fipa-bit-cp \
		  -fipa-cp \
		  -fipa-icf \
		  -fipa-ra \
		  -fipa-sra \
		  -fipa-vrp \
		  -fisolate-erroneous-paths-dereference \
		  -flra-remat \
		  -finline-functions \
		  -finline-small-functions \
		  -findirect-inlining \
		  -fpeephole2 \
		  -freorder-blocks-and-partition \
		  -frerun-cse-after-loop  \
		  -fschedule-insns \
		  -fschedule-insns2 \
		  -fsched-interblock \
		  -fsched-spec \
		  -fstore-merging \
		  -fstrict-aliasing \
		  -fthread-jumps \
		  -ftree-builtin-call-dce \
		  -ftree-loop-vectorize \
		  -ftree-pre \
		  -ftree-slp-vectorize \
		  -ftree-switch-conversion \
		  -ftree-tail-merge \
		  -ftree-vrp \
		  -fvect-cost-model=very-cheap \
		  -foptimize-sibling-calls \
		  -foptimize-strlen \
		  -fpartial-inlining
endif

OBJCOPYFLAGS := -O binary \
				--strip-unneeded

SRCS_ASM := ${wildcard ${CURDIR}/asm/*.asm}
SRCS_C   := ${wildcard ${CURDIR}/c/*.c}

OBJS_ASM := ${subst ${CURDIR}/asm,${OBJ_DIR},${SRCS_ASM:.asm=.o}}
OBJS_C   := ${subst ${CURDIR}/c,${OBJ_DIR},${SRCS_C:.c=.o}}

HDRS := ${OBJS_C:.o=.d}

.PHONY: all build clean format

all: format build

format: ${SRCS_C} ${SRCS_H}
	clang-format -i $^

build: ${FULLEXEC}

clean:
	${RM} ${OBJS_ASM} ${OBJS_C} ${HDRS}

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

-include ${HDRS}
