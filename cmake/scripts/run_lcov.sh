#!/bin/bash

PROJECT_SOURCE_DIR=$1
CMAKE_BINARY_DIR=$2

LCOV_REPORT_DIR=$PROJECT_SOURCE_DIR/reports/lcov_coverage
SOURCE_DIR=$PROJECT_SOURCE_DIR/src
RAWINFO_DIR=$CMAKE_BINARY_DIR/CMakeFiles/nanoservices2_testing.dir/src
GCOV_TOOL=`which gcov-11`

echo ==== run_lcov.sh:  Preparing to run tests with coverage
pushd . > /dev/null
echo ---- run_lcov.sh:  PROJECT_SOURCE_DIR $PROJECT_SOURCE_DIR
echo ---- run_lcov.sh:  CMAKE_BINARY_DIR $CMAKE_BINARY_DIR
echo ---- run_lcov.sh:  SOURCE_DIR $SOURCE_DIR
echo ---- run_lcov.sh:  RAWINFO_DIR $RAWINFO_DIR
echo ---- run_lcov.sh:  GCOV_TOOL $GCOV_TOOL

echo ==== run_lcov.sh:  Cleaning existing raw coverage data in binary dir $CMAKE_BINARY_DIR
cd $CMAKE_BINARY_DIR
echo ---- run_lcov.sh:  In directory: `pwd`

FILES=`find . -type f`
FILE_REGEX="(\.gcda|\.gcov)$"
for F in $FILES; do
  if [[ ${F} =~ $FILE_REGEX ]]; then
    echo .... run_lcov.sh:  Deleting $F
    rm $F
  fi
done
echo ==== run_lcov.sh:  Existing raw coverage data deleted

echo ==== run_lcov.sh:  Running tests with coverage
cd $SOURCE_DIR
$CMAKE_BINARY_DIR/nanoservices2_testing
echo ==== run_lcov.sh:  Tests run, return code: $?

echo ==== run_lcov.sh:  Running lcov
mkdir -p $LCOV_REPORT_DIR
rm -R $LCOV_REPORT_DIR/*
cd $LCOV_REPORT_DIR
echo ---- run_lcov.sh:  In directory: `pwd`

lcov --capture --gcov-tool $GCOV_TOOL --directory $RAWINFO_DIR/ --output-file $LCOV_REPORT_DIR/coverage.info
mkdir $LCOV_REPORT_DIR/html
genhtml $LCOV_REPORT_DIR/coverage.info --output-directory $LCOV_REPORT_DIR/html
echo ==== run_lcov.sh:  lcov reports placed in $LCOV_REPORT_DIR

popd > /dev/null
echo ==== run_lcov.sh:  Coverage report script finished