# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

EXEC     := libutils.a
FULLEXEC := ${LIB_DIR}/${EXEC}

SRCS := ${wildcard ${CURDIR}/*.c}
OBJS := ${subst ${CURDIR},${OBJ_DIR},${SRCS:.c=.utils.c.o}}

LIBGCCDIR := ${shell dirname `${CC} -print-libgcc-file-name`}

CFLAGS += -fno-stack-check \
		  -ffreestanding \
		  -nostdlib

CPPFLAGS += -I ${SRC_DIR}/include/common \
			-MMD

ARFLAGS := crs

.PHONY: all build format clean

all: format build

build:
	@echo Building libutils sources...
	@${MAKE} ${FULLEXEC}

format: ${SRCS}
	@echo Formatting libutils sources...
	@clang-format -i $^

clean:
	@echo Cleaning up libutils binaries.
	@${RM} ${OBJS} ${FULLEXEC}

${FULLEXEC}: ${OBJS}
	@echo Archiving objects into a library...
	@${AR} \
		${ARFLAGS} \
		$@ \
		$^

${OBJ_DIR}/%.utils.c.o: ${CURDIR}/%.c
	@echo Compiling libutils: ${notdir $<}...
	@${CC} \
		${CFLAGS} \
		${CPPFLAGS} \
		-c \
		-o $@ \
		$<
