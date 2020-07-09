#!/bin/sh
if [ -z "${VCPKG_ROOT}" ]; then
	echo 'Please specify VCPKG_ROOT environment variable (for example: home/user/vcpkg)'
else
    mkdir -p build && cd build && cmake -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake .. && cd ..;
fi
