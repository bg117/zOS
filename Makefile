export ROOT_DIR=${CURDIR}

export BUILD_DIR=${CURDIR}/build

export BIN_DIR=${BUILD_DIR}/bin
export OBJ_DIR=${BUILD_DIR}/obj

.PHONY: all build clean check-dirs

all: check-dirs build

build:
	${MAKE} -C ${CURDIR}/src

clean:
	${RM} -r ${BUILD_DIR}

check-dirs:
	-mkdir -p ${BIN_DIR}
	-mkdir -p ${OBJ_DIR}
