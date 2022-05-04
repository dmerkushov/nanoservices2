#!/bin/bash

PROJECT_SOURCE_DIR=$1
CMAKE_BINARY_DIR=$2

GCOV_REPORT_DIR=$PROJECT_SOURCE_DIR/reports/gcov_coverage
LCOV_REPORT_DIR=$PROJECT_SOURCE_DIR/reports/lcov_coverage
SOURCE_DIR=$PROJECT_SOURCE_DIR/src
RAWINFO_DIR=$CMAKE_BINARY_DIR/CMakeFiles/nanoservices2_testing.dir/src
GCOV_TOOL=`which gcov-11`

echo ==== run_gcov.sh: Preparing to run tests with coverage
pushd . > /dev/null
echo ---- run_gcov.sh: PROJECT_SOURCE_DIR $PROJECT_SOURCE_DIR
echo ---- run_gcov.sh: CMAKE_BINARY_DIR $CMAKE_BINARY_DIR
echo ---- run_gcov.sh: SOURCE_DIR $SOURCE_DIR
echo ---- run_gcov.sh: RAWINFO_DIR $RAWINFO_DIR
echo ---- run_gcov.sh: GCOV_TOOL $GCOV_TOOL

echo ==== run_gcov.sh: Cleaning existing raw coverage data in binary dir $CMAKE_BINARY_DIR
cd $CMAKE_BINARY_DIR
echo ---- run_gcov.sh: In directory: `pwd`

FILES=`find . -type f`
FILE_REGEX="(\.gcda|\.gcov)$"
for F in $FILES; do
  if [[ ${F} =~ $FILE_REGEX ]]; then
    echo .... run_gcov.sh: Deleting $F
    rm $F
  fi
done
echo ==== run_gcov.sh: Existing raw coverage data deleted

echo ==== run_gcov.sh: Running tests with coverage
cd $SOURCE_DIR
$CMAKE_BINARY_DIR/nanoservices2_testing
echo ==== run_gcov.sh: Tests run, return code: $?

echo ==== run_gcov.sh: Running gcov
cd $RAWINFO_DIR
echo ---- run_gcov.sh: In directory: `pwd`

FILES=`find . -type f`
FILE_REGEX="(\.gcda)$"
for F in $FILES; do
  if [[ ${F} =~ $FILE_REGEX ]]; then
    echo .... run_gcov.sh: gcov processing $SOURCE_DIR/$F

    # When running GCOV, we need $PROJECT_SOURCE_DIR as the source dir, as GCOV
    # prints source paths in its result files based on that dir
    $GCOV_TOOL -l -p -r -s "$PROJECT_SOURCE_DIR" $F
  fi
done
echo ==== run_gcov.sh: gcov finished

echo ==== run_gcov.sh: Moving gcov reports to $GCOV_REPORT_DIR
mkdir -p $GCOV_REPORT_DIR
rm -r $GCOV_REPORT_DIR/*
echo ---- run_gcov.sh: GCOV report dir prepared

cd $RAWINFO_DIR
echo ---- run_gcov.sh: In directory: `pwd`

FILES=`find . -type f`
FILE_REGEX="(\.gcov)$"
for F in $FILES; do
  if [[ ${F} =~ $FILE_REGEX ]]; then
    echo .... run_gcov.sh: Processing $F
    mv $F $GCOV_REPORT_DIR
  fi
done
echo ==== run_gcov.sh: gcov reports placed in $GCOV_REPORT_DIR

popd > /dev/null
echo ==== run_gcov.sh: Coverage report script finished