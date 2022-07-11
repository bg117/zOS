# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

EXEC     := example.mod
FULLEXEC := ${MODULES_BIN_DIR}/${EXEC}

CC := i686-elf-gcc
LD := i686-elf-ld

LIBGCCDIR := ${shell dirname `${CC} -print-libgcc-file-name`}

SRCS := ${wildcard ${CURDIR}/*.c}
OBJS := ${subst ${CURDIR},${MODULES_OBJ_DIR}/example,${SRCS:.c=.o}}

CFLAGS := -Wall \
		  -Wextra \
		  -fno-stack-check \
		  -ffreestanding \
		  -fpic \
		  -nostdlib

CPPFLAGS := -MMD

LDFLAGS := -fpic \
		   -shared \
		   -L${LIBGCCDIR} \
		   -nostdlib

HDRS := ${OBJS:.o=.d}

.PHONY: all build clean check-dirs

all: check-dirs build

build: ${FULLEXEC}

clean:
	${RM} ${OBJS} ${HDRS}

check-dirs:
	-mkdir -p ${MODULES_OBJ_DIR}/example

${FULLEXEC}: ${OBJS}
	${LD} \
		${LDFLAGS} \
		-o $@ \
		$^ \
		-lgcc

${MODULES_OBJ_DIR}/example/%.o: ${CURDIR}/%.c
	${CC} \
		${CFLAGS} \
		${CPPFLAGS} \
		-c \
		-o $@ \
		$<

-include ${HDRS}
