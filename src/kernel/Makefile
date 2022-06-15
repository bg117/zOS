ASM      := nasm
ASMFLAGS := -f obj

CC := owcc
LD := wlink

CFLAGS := -Wall \
          -Wextra \
          -mcmodel=t \
          -mabi=cdecl \
          -fno-rtti \
          -O0 \
          -g2

SRCS_ASM := ${wildcard ${CURDIR}/*.asm}
SRCS_C   := ${wildcard ${CURDIR}/*.c}

OBJS_ASM := ${subst ${CURDIR},${OBJ_DIR},${SRCS_ASM:.asm=.asm.obj}}
OBJS_C   := ${subst ${CURDIR},${OBJ_DIR},${SRCS_C:.c=.c.obj}}

EXEC     := zs
FULLEXEC := ${BIN_DIR}/${EXEC}

.PHONY: all build clean

all: build

build: ${FULLEXEC}

clean:
	${RM} ${OBJS_ASM} ${OBJS_C}

${FULLEXEC}: ${OBJS_ASM} ${OBJS_C}

${OBJ_DIR}/%.c.obj: ${CURDIR}/%.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c -o $@ $<

${OBJ_DIR}/%.asm.obj: ${CURDIR}/%.asm
	${ASM} ${ASMFLAGS} -o $@ $<
