#!/bin/bash

PROJECT_SOURCE_DIR=$1
CMAKE_BINARY_DIR=$2

GCOV_COVERAGE_DIR=$PROJECT_SOURCE_DIR/gcov_coverage

echo ==== Preparing to run tests with coverage
echo ---- PROJECT_SOURCE_DIR $PROJECT_SOURCE_DIR
echo ---- CMAKE_BINARY_DIR $CMAKE_BINARY_DIR

echo ==== Running tests with coverage
./nanoservices2_testing
echo ==== Tests run, return code: $?

echo ==== Collecting gcov coverage data

mkdir -p $GCOV_COVERAGE_DIR
rm -R $GCOV_COVERAGE_DIR/*
cp -R $CMAKE_BINARY_DIR/CMakeFiles/nanoservices2_testing.dir/* $GCOV_COVERAGE_DIR

pushd .
cd $GCOV_COVERAGE_DIR
FILES=`find . -type f`
GCOV_FILE_REGEX="(gcda|gcno)$"
echo ---- Files: $FILES
for F in $FILES; do
  if [[ ! ${F} =~ $GCOV_FILE_REGEX ]]; then
    echo .... Deleting non-conforming file: $F
    rm $F
  else
    echo .... Keeping conforming file: $F
  fi
done
popd

echo ==== gcov coverage data is placed at $GCOV_COVERAGE_DIR