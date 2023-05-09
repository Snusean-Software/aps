#!/bin/bash
PROJECT_ROOT_DIR=$(pwd)

mkdir -p ${PROJECT_ROOT_DIR}/build && cd ${PROJECT_ROOT_DIR}/build

# Build
cmake .. -DCMAKE_BUILD_TESTS=True
make
if [[ $? != "0" ]]; then
     echo "[ ERROR ] Build has been failed"
     exit 1
fi
echo 

echo "[ SUCCESS ] Project has been successfully builded and tested"
exit 0
