# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

EXEC     := zs
FULLEXEC := ${BIN_DIR}/${EXEC}

LIBGCCDIR := ${shell dirname `${CC} -print-libgcc-file-name`}

ASFLAGS := -f elf32

CFLAGS += -fno-stack-check \
		  -ffreestanding \
		  -nostdlib

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

SRCS_ASM := ${wildcard ${CURDIR}/asm/*.asm}
SRCS_C   := ${wildcard ${CURDIR}/c/*.c}
SRCS_H   := ${wildcard ${SRC_DIR}/include/common/kernel/*.h}

OBJS_ASM := ${subst ${CURDIR}/asm,${OBJ_DIR},${SRCS_ASM:.asm=.o}}
OBJS_C   := ${subst ${CURDIR}/c,${OBJ_DIR},${SRCS_C:.c=.o}}

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

${OBJ_DIR}/%.o: ${CURDIR}/c/%.c
	@echo Compiling kernel: ${notdir $<}
	@${CC} \
		${CFLAGS} \
		${CPPFLAGS} \
		-c \
		-o $@ \
		$<

${OBJ_DIR}/%.o: ${CURDIR}/asm/%.asm
	@echo Assembling kernel: ${notdir $<}
	@${AS} \
		${ASFLAGS} \
		-o $@ \
		$<

-include ${HDRS}
