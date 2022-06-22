ASM      := ia16-elf-as
ASMFLAGS :=

CC := ia16-elf-gcc
LD := ia16-elf-gcc

LIBGCC_DIR := ${shell dirname `${CC} -print-libgcc-file-name`}

CFLAGS   := -std=gnu99 \
			-Wall \
			-Wextra \
			-Os \
			-fno-omit-frame-pointer \
			-fno-stack-check \
			-ffreestanding \
			-nostdlib
CPPFLAGS := -I${CURDIR}/include \
			-MMD
LDFLAGS  := -nostartfiles \
			-Wl,-Map=${BUILD_DIR}/stage2.map \
			-Wl,-T${CURDIR}/linker.ld

SRCS_S := ${wildcard ${CURDIR}/*.S}
SRCS_C := ${wildcard ${CURDIR}/*.c}

OBJS_S := ${subst ${CURDIR},${OBJ_DIR},${SRCS_S:.S=.S.o}}
OBJS_C := ${subst ${CURDIR},${OBJ_DIR},${SRCS_C:.c=.c.o}}

HDRS := ${OBJS_C:.o=.d}

EXEC     := zs
FULLEXEC := ${BIN_DIR}/${EXEC}

.PHONY: all build clean

all: build

build: ${FULLEXEC}

clean:
	${RM} ${OBJS_S} ${OBJS_C}

${FULLEXEC}: ${OBJS_S} ${OBJS_C}
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

${OBJ_DIR}/%.S.o: ${CURDIR}/%.S
	${ASM} \
		${ASMFLAGS} \
		-o $@ \
		$<
