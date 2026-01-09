CMAKE_COMMON_FLAGS ?= -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE_DEBUG_FLAGS ?= ${CMAKE_COMMON_FLAGS} -DCMAKE_BUILD_TYPE=DEBUG
CMAKE_RELEASE_FLAGS ?= ${CMAKE_COMMON_FLAGS} -DCMAKE_BUILD_TYPE=RELEASE

build-debug build-release: %: %
	mkdir $*

.PHONY: cmake-debug
cmake-debug: build-debug
	cmake -S . -B $< ${CMAKE_DEBUG_FLAGS}
	if [ -e ./$</compile_commands.json ]; then cp ./$</compile_commands.json .; fi

.PHONY: cmake-release
cmake-release: build-release
	cmake -S . -B $< ${CMAKE_RELEASE_FLAGS}
	if [ -e ./$</compile_commands.json ]; then cp ./$</compile_commands.json .; fi

.PHONY: clean
clean:
	rm -rf ./build-*
