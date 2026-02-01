.PHONY: build release run test clean install update

.DEFAULT_GOAL := run

build:
	mkdir -p build
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ..
	cd build && make -j

release:
	mkdir -p build
	cd build && cmake -G "Unix Makefiles" ..
	cd build && make -j

run: build
	./build/aprsfmt -s N0CALL-10 -d APRS -p WIDE2-2 -i "Hello World"

test: build
	./build/aprsfmt_test

clean:
	rm -rf build

install: release
	sudo make -C build install

update:
	git pull
	git submodule update --init --recursive
