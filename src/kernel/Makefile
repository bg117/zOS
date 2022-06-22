ASM      := nasm
ASMFLAGS := -f elf

CC := ia16-elf-gcc
LD := ia16-elf-gcc

LIBGCC_DIR := ${shell dirname `${CC} -print-libgcc-file-name`}

CFLAGS   := -std=gnu99 \
			-Wall \
			-Wextra \
			-Os \
			-fno-omit-frame-pointer \
			-fno-stack-check \
			-ffreestanding
CPPFLAGS := -I${CURDIR}/include \
			-MMD
LDFLAGS  := -nostartfiles \
			-flto \
			-Wl,-Map=${BUILD_DIR}/stage2.map \
			-Wl,-T${CURDIR}/linker.ld

SRCS_ASM := ${wildcard ${CURDIR}/*.asm}
SRCS_C   := ${wildcard ${CURDIR}/*.c}

OBJS_ASM := ${subst ${CURDIR},${OBJ_DIR},${SRCS_ASM:.asm=.asm.o}}
OBJS_C   := ${subst ${CURDIR},${OBJ_DIR},${SRCS_C:.c=.c.o}}

HDRS := ${OBJS_C:.o=.d}

EXEC     := zs
FULLEXEC := ${BIN_DIR}/${EXEC}

.PHONY: all build clean

all: build

build: ${FULLEXEC}

clean:
	${RM} ${OBJS_ASM} ${OBJS_C}

${FULLEXEC}: ${OBJS_ASM} ${OBJS_C}
	${LD} \
		${LDFLAGS} \
		-o $@ \
		$^

-include ${HDRS}

${OBJ_DIR}/%.c.o: ${CURDIR}/%.c
	${CC} \
		${CFLAGS} \
		${CPPFLAGS} \
		-c \
		-o $@ \
		$<

${OBJ_DIR}/%.asm.o: ${CURDIR}/%.asm
	${ASM} \
		${ASMFLAGS} \
		-o $@ \
		$<
