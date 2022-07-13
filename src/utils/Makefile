# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

EXEC     := libutils.a
FULLEXEC := ${LIB_DIR}/${EXEC}

CC := i686-elf-gcc
AR := i686-elf-ar

SRCS := ${wildcard ${CURDIR}/*.c}
OBJS := ${subst ${CURDIR},${OBJ_DIR},${SRCS:.c=.utils.o}}

LIBGCCDIR := ${shell dirname `${CC} -print-libgcc-file-name`}

CFLAGS += -Wall \
		  -Wextra \
		  -fno-stack-check \
		  -ffreestanding \
		  -nostdlib

CPPFLAGS := -I ${SRC_DIR}/include/common \
			-MMD

ARFLAGS := crs

.PHONY: all build format clean

all: build

build: ${FULLEXEC}

format: ${SRCS}
	clang-format -i $^

clean:
	${RM} ${OBJS}

${FULLEXEC}: ${OBJS}
	${AR} \
		${ARFLAGS} \
		$@ \
		$^

${OBJ_DIR}/%.utils.o: ${CURDIR}/%.c
	${CC} \
		${CFLAGS} \
		${CPPFLAGS} \
		-c \
		-o $@ \
		$<
