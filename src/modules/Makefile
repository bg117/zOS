# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

# To use this makefile, set the MOD variable to point to the
# directory of the module you want to build. This is similar to
# the process of building Linux modules.
#
# Then, call `make build-module MOD=directory'.
#
# There is also an alternate target, `build-all' which calls
# `build-module' on each directory inside this directory, `modules'.

export MODULES_BIN_DIR=${BIN_DIR}/modules
export MODULES_OBJ_DIR=${OBJ_DIR}/modules

ifdef MOD
RPATH = ${shell realpath ${MOD}}

EXEC     = ${shell basename ${RPATH}}.mod
FULLEXEC = ${MODULES_BIN_DIR}/${EXEC}

SRCS = ${wildcard ${RPATH}/*.c}
OBJS = ${subst ${RPATH},${MODULES_OBJ_DIR}/${EXEC},${SRCS:.c=.o}}

HDRS = ${OBJS:.o=.d}
endif

LIBGCCDIR := ${shell dirname `${CC} -print-libgcc-file-name`}

CFLAGS += -fno-stack-check \
		  -ffreestanding \
		  -fpic \
		  -nostdlib

CPPFLAGS := -I ${SRC_DIR}/include/common \
			-MMD

LDFLAGS := -fpic \
		   -shared \
		   -L ${LIBGCCDIR} \
		   -L ${LIB_DIR} \
		   -nostdlib

.PHONY: build-all build-module format check-dirs clean

build-all:
	@echo Building all modules in current directory...

	@for i in ${CURDIR}/*/; do \
		${MAKE} build-module MOD=$$i; \
	done

build-module:
	@echo Building module: ${EXEC}

	@${MAKE} check-dirs
	@${MAKE} ${FULLEXEC}

format: ${SRCS}
	@echo Formatting source files...
	@clang-format -i $^

check-dirs:
	@echo Checking if module directories exist...

	@-mkdir -p ${MODULES_BIN_DIR}
	@-mkdir -p ${MODULES_OBJ_DIR}/${EXEC}

clean:
	@echo Cleaning up module directories...
	@${RM} -r ${MODULES_BIN_DIR} ${MODULES_OBJ_DIR}

${FULLEXEC}: ${OBJS}
	@echo Linking module...
	@${LD} \
		${LDFLAGS} \
		-o $@ \
		$^ \
		-lutils \
		-lgcc

${MODULES_OBJ_DIR}/${EXEC}/%.o: ${RPATH}/%.c
	@echo Compiling $<...
	@${CC} \
		${CFLAGS} \
		${CPPFLAGS} \
		-c \
		-o $@ \
		$<

-include ${HDRS}
