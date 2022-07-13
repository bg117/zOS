# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

export MODULES_BIN_DIR=${BIN_DIR}/modules
export MODULES_OBJ_DIR=${OBJ_DIR}/modules

RPATH := ${shell realpath ${MOD}}

EXEC     := ${shell basename ${RPATH}}.mod
FULLEXEC := ${MODULES_BIN_DIR}/${EXEC}

CC      := i686-elf-gcc
LD      := i686-elf-ld

LIBGCCDIR := ${shell dirname `${CC} -print-libgcc-file-name`}

SRCS := ${wildcard ${RPATH}/*.c}
OBJS := ${subst ${RPATH},${MODULES_OBJ_DIR}/${EXEC},${SRCS:.c=.o}}

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

.PHONY: build-all build-module check-dirs clean

build-all:
	for i in ${CURDIR}/*/; do \
		${MAKE} build-module MOD=$$i; \
	done

build-module: check-dirs ${FULLEXEC}
# MOD must not be empty
	echo ${RPATH}
	echo ${EXEC}
	echo ${FULLEXEC}
	echo ${SRCS}

check-dirs:
	-mkdir -p ${MODULES_BIN_DIR}
	-mkdir -p ${MODULES_OBJ_DIR}/${EXEC}

clean:
	-rm -r ${MODULES_BIN_DIR} ${MODULES_OBJ_DIR}

${FULLEXEC}: ${OBJS}
	${LD} \
		${LDFLAGS} \
		-o $@ \
		$^ \
		-lgcc

${MODULES_OBJ_DIR}/${EXEC}/%.o: ${RPATH}/%.c
	${CC} \
		${CFLAGS} \
		${CPPFLAGS} \
		-c \
		-o $@ \
		$<

-include ${HDRS}
