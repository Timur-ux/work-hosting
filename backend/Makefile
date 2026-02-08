CMAKE_COMMON_FLAGS ?= -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DENABLE_CPACK=OFF -DWITH_PERF_TOOL=OFF -DZMQ_BUILD_TESTS=OFF -DCPPZMQ_BUILD_TESTS=OFF
CMAKE_DEBUG_FLAGS ?= ${CMAKE_COMMON_FLAGS} -DCMAKE_BUILD_TYPE=DEBUG
CMAKE_RELEASE_FLAGS ?= ${CMAKE_COMMON_FLAGS} -DCMAKE_BUILD_TYPE=RELEASE 
NPROC ?= $(shell nproc)

all: release

build-debug build-release:
	mkdir $@

.PHONY: cmake-debug
cmake-debug: build-debug
	cmake -S . -B $< ${CMAKE_DEBUG_FLAGS} -DBUILD_ALL=ON
	if [ -e ./$</compile_commands.json ]; then cp ./$</compile_commands.json .; fi

.PHONY: cmake-release
cmake-release: build-release
	cmake -S . -B $< ${CMAKE_RELEASE_FLAGS} -DBUILD_ALL=ON
	if [ -e ./$</compile_commands.json ]; then cp ./$</compile_commands.json .; fi

.PHONY: debug release
debug release: %: build-% cmake-%
	cmake --build build-$* -- -j$(NPROC)

cmake-release-target-%: build-release
	cmake -S . -B $< ${CMAKE_RELEASE_FLAGS} -DBUILD_$*=ON

release-target-%: cmake-release-target-%
	cmake --build build-release --target $* -- -j$(NPROC)

debug-target-%: cmake-debug
	cmake --build build-debug --target $* -- -j$(NPROC)

trdparty-only: build-release
	cmake -S . -B $< ${CMAKE_RELEASE_FLAGS} -DBUILD_ALL=ON -DTRDPARTY_ONLY=ON

.PHONY: clean
clean:
	rm -rf ./build-*

