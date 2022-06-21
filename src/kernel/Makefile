ASM      := ia16-elf-as
ASMFLAGS :=

CC := ia16-elf-gcc
LD := ia16-elf-gcc

CFLAGS   := -std=gnu99 \
			-Wall \
			-Wextra \
			-g \
			-O0 \
			-fno-omit-frame-pointer \
			-fno-stack-check
CPPFLAGS := -I${CURDIR}/include
LDFLAGS  := -lgcc \
			-nostartfiles

SRCS_S := ${wildcard ${CURDIR}/*.S}
SRCS_C := ${wildcard ${CURDIR}/*.c}

OBJS_S := ${subst ${CURDIR},${OBJ_DIR},${SRCS_S:.S=.S.o}}
OBJS_C := ${subst ${CURDIR},${OBJ_DIR},${SRCS_C:.c=.c.o}}

EXEC     := zs
FULLEXEC := ${BIN_DIR}/${EXEC}

${info ${SRCS_S}}

.PHONY: all build clean

all: build

build: ${FULLEXEC}

clean:
	${RM} ${OBJS_S} ${OBJS_C}

${FULLEXEC}: ${OBJS_S} ${OBJS_C}
	${LD} \
		${LDFLAGS} \
		-Wl,-Map=${BUILD_DIR}/stage2.map \
		-Wl,-T -Wl,linker.ld \
		-o $@ \
		$^

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
