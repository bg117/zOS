# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

EXEC     := zs
FULLEXEC := ${BIN_DIR}/${EXEC}

LIBGCCDIR := ${shell dirname `${CC} -print-libgcc-file-name`}

ASFLAGS := -f elf32

CFLAGS += -fno-stack-check \
		  -fno-strict-aliasing \
		  -ffreestanding \
		  -nostdlib \
		  -std=gnu11

CPPFLAGS := -I ${SRC_DIR}/include/common \
			-MMD \
			-Wno-unknown-pragmas

LDFLAGS := -Map ${OBJ_DIR}/${EXEC}.map \
		   -T ${CURDIR}/linker.ld \
		   -L ${LIBGCCDIR} \
		   -L ${LIB_DIR} \
		   -nostdlib

OBJCOPYFLAGS := -O binary \
				--strip-unneeded

SRCS_ASM := ${wildcard ${CURDIR}/*.asm}
SRCS_C   := ${wildcard ${CURDIR}/*.c}
SRCS_H   := ${wildcard ${SRC_DIR}/include/common/kernel/*.h}

OBJS_ASM := ${subst ${CURDIR},${OBJ_DIR},${SRCS_ASM:.asm=.k.asm.o}}
OBJS_C   := ${subst ${CURDIR},${OBJ_DIR},${SRCS_C:.c=.k.c.o}}

HDRS := ${OBJS_C:.o=.d}

.PHONY: all build clean format

all: format build

format: ${SRCS_C} ${SRCS_H}
	@echo Formatting kernel sources...
	@clang-format -i $^

build:
	@echo Building kernel...
	@${MAKE} ${FULLEXEC}

clean:
	@echo Cleaning up kernel binaries...
	@${RM} ${OBJS_ASM} ${OBJS_C} ${HDRS} ${FULLEXEC} ${FULLEXEC}.r

${FULLEXEC}: ${FULLEXEC}.r
	@echo Creating final kernel binary...
	@${OBJCOPY} \
		${OBJCOPYFLAGS} \
		$< \
		$@

${FULLEXEC}.r: ${OBJS_ASM} ${OBJS_C}
	@echo Linking into temporary ELF object...
	@${LD} \
		${LDFLAGS} \
		-o $@ \
		$^ \
		-lutils \
		-lgcc

${OBJ_DIR}/%.k.c.o: ${CURDIR}/%.c
	@echo Compiling kernel: ${notdir $<}
	@${CC} \
		${CFLAGS} \
		${CPPFLAGS} \
		-c \
		-o $@ \
		$<

${OBJ_DIR}/%.k.asm.o: ${CURDIR}/%.asm
	@echo Assembling kernel: ${notdir $<}
	@${AS} \
		${ASFLAGS} \
		-o $@ \
		$<

-include ${HDRS}
