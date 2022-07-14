# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

export ROOT_DIR=${CURDIR}

export BUILD_DIR=${CURDIR}/build

export BIN_DIR=${BUILD_DIR}/bin
export LIB_DIR=${BUILD_DIR}/lib
export OBJ_DIR=${BUILD_DIR}/obj

export SYSROOT=${BUILD_DIR}/root

.PHONY: all build clean check-dirs

all: check-dirs build

build:
	${MAKE} -C ${CURDIR}/src

clean:
	${RM} -r ${BUILD_DIR}

check-dirs:
	-mkdir -p ${BIN_DIR}
	-mkdir -p ${OBJ_DIR}
	-mkdir -p ${LIB_DIR}
	-mkdir -p ${SYSROOT}
