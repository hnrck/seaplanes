# Compilers Compilers configuration, alternatives: gcc, g++, ...
CC ?= clang
CXX ?= clang++

# CMake generator configuration, alternatives: unix makefiles, ...
GENERATOR ?= Ninja

# CMake build type configuration, alternatives: release, debug,  relwithminsize
BUILD_TYPE ?= RelWithDebInfo

# Directories for building and installing, alternatives: wherever you want
BUILD_DIR ?= build
INSTALL_DIR ?= install

.PHONY: all gen build install test format lint clean

all: build

# Generate CMake build environment
gen:
	cmake -G${GENERATOR} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_C_COMPILER=${CC} -DCMAKE_CXX_COMPILER=${CXX} -DCMAKE_CXX_STANDARD=${CXXSTD} -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} -DCPPCHECK=1 -H. -B ${BUILD_DIR}

# Build from CMake build environment
build: gen
	cmake --build ${BUILD_DIR} --target all

# Install built library and headers
install: build
	cmake --build ${BUILD_DIR} --target ${@}

# Format files
format: gen
	cmake --build ${BUILD_DIR} --target ${@}

# Static code checking
lint: gen
	cmake --build ${BUILD_DIR} --target ${@}

# Generate doc files
doc: gen
	cmake --build ${BUILD_DIR} --target ${@}

refman: doc
	make -C ${BUILD_DIR}/doc/latex

# Remove build environment
clean:
	rm -rf ${BUILD_DIR}
