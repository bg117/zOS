ASM      := nasm
ASMFLAGS := -f obj

CC := wcc
LD := wlink

CFLAGS := -3 \
          -d3 \
          -s \
          -wx \
          -ms \
          -zl \
          -zq \
          -oneaszxhmpl+

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
	${LD} \
		NAME $@ \
		FILE { $^ } \
		OPTION \
			MAP=${BUILD_DIR}/stage2.map, \
			NOEXTENSION \
		@linker.wlink

${OBJ_DIR}/%.c.obj: ${CURDIR}/%.c
	${CC} ${CFLAGS} ${CPPFLAGS} -fo=$@ $<

${OBJ_DIR}/%.asm.obj: ${CURDIR}/%.asm
	${ASM} ${ASMFLAGS} -o $@ $<
