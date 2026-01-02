CMAKE_COMMON_FLAGS ?= -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE_DEBUG_FLAGS ?= ${CMAKE_COMMON_FLAGS} -DCMAKE_BUILD_TYPE=DEBUG
CMAKE_RELEASE_FLAGS ?= ${CMAKE_COMMON_FLAGS} -DCMAKE_BUILD_TYPE=RELEASE

build-debug build-release: %: %
	mkdir $*

.PHONY: cmake-debug
cmake-debug: build-debug
	cmake -S . -B $< ${CMAKE_DEBUG_FLAGS}
	cp ./$</compile_commands.json .

.PHONY: cmake-release
cmake-release: build-release
	cmake -S . -B $< ${CMAKE_RELEASE_FLAGS}
	cp ./$</compile_commands.json .

.PHONY: clean
clean:
	rm -rf ./build-*
