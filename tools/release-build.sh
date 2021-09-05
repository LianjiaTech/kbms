#!/bin/sh

# Run this script at the directory which holds it.

set -x

SOURCE_DIR=`pwd`/..
BUILD_DIR=${SOURCE_DIR}/build

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR \
  && cd $BUILD_DIR \
  && cmake .. \
  && make -j5 


PACKAGE_DIR=${SOURCE_DIR}/kbms
rm -rf $PACKAGE_DIR
mkdir -p $PACKAGE_DIR
mv ${BUILD_DIR}/lib ${PACKAGE_DIR}/
cp -r ${SOURCE_DIR}/src ${PACKAGE_DIR}/include
find ${PACKAGE_DIR}/include/ -name *.cc | xargs rm -rf
find ${PACKAGE_DIR}/include/ -name CMakeLists* |xargs rm -rf
find ${PACKAGE_DIR}/include/ -name *.md | xargs rm -rf
