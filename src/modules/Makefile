# Copyright (c) 2022 bg117
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

export MODULES_BIN_DIR=${BIN_DIR}/modules
export MODULES_OBJ_DIR=${OBJ_DIR}/modules

.PHONY: all example check-dirs

all: check-dirs example

example:
	${MAKE} -C ${CURDIR}/example

check-dirs:
	-mkdir -p ${MODULES_BIN_DIR}
	-mkdir -p ${MODULES_OBJ_DIR}
