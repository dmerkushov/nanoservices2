#!/bin/bash

PROJECT_SOURCE_DIR=$1
CMAKE_BINARY_DIR=$2

GCOV_REPORT_DIR=$PROJECT_SOURCE_DIR/reports/gcov_coverage
LCOV_REPORT_DIR=$PROJECT_SOURCE_DIR/reports/lcov_coverage
SOURCE_DIR=$PROJECT_SOURCE_DIR/src
RAWINFO_DIR=$CMAKE_BINARY_DIR/CMakeFiles/nanoservices2_testing.dir/src
GCOV_TOOL=`which gcov-11`

echo ==== Preparing to run tests with coverage
pushd . > /dev/null
echo ---- PROJECT_SOURCE_DIR $PROJECT_SOURCE_DIR
echo ---- CMAKE_BINARY_DIR $CMAKE_BINARY_DIR
echo ---- SOURCE_DIR $SOURCE_DIR
echo ---- RAWINFO_DIR $RAWINFO_DIR
echo ---- GCOV_TOOL $GCOV_TOOL

echo ==== Cleaning existing raw coverage data in binary dir $CMAKE_BINARY_DIR
cd $CMAKE_BINARY_DIR
echo ---- In directory: `pwd`

FILES=`find . -type f`
FILE_REGEX="(\.gcda|\.gcov)$"
for F in $FILES; do
  if [[ ${F} =~ $FILE_REGEX ]]; then
    echo .... Deleting $F
    rm $F
  fi
done
echo ==== Existing raw coverage data deleted

echo ==== Running tests with coverage
cd $SOURCE_DIR
$CMAKE_BINARY_DIR/nanoservices2_testing
echo ==== Tests run, return code: $?

echo ==== Running gcov
cd $RAWINFO_DIR
echo ---- In directory: `pwd`

FILES=`find . -type f`
FILE_REGEX="(\.gcda)$"
for F in $FILES; do
  if [[ ${F} =~ $FILE_REGEX ]]; then
    echo .... gcov processing $SOURCE_DIR/$F

    # When running GCOV, we need $PROJECT_SOURCE_DIR as the source dir, as GCOV
    # prints source paths in its result files based on that dir
    $GCOV_TOOL -l -p -r -s "$PROJECT_SOURCE_DIR" $F
  fi
done
echo ==== gcov finished

echo ==== Moving gcov reports to $GCOV_REPORT_DIR
mkdir -p $GCOV_REPORT_DIR
rm -r $GCOV_REPORT_DIR/*
echo ---- GCOV report dir prepared

cd $RAWINFO_DIR
echo ---- In directory: `pwd`

FILES=`find . -type f`
FILE_REGEX="(\.gcov)$"
for F in $FILES; do
  if [[ ${F} =~ $FILE_REGEX ]]; then
    echo .... Processing $F
    mv $F $GCOV_REPORT_DIR
  fi
done
echo ==== gcov reports placed in $GCOV_REPORT_DIR

echo ==== Running lcov
mkdir -p $LCOV_REPORT_DIR
rm -R $LCOV_REPORT_DIR/*
cd $LCOV_REPORT_DIR
echo ---- In directory: `pwd`

lcov --capture --gcov-tool $GCOV_TOOL --directory $RAWINFO_DIR/ --output-file $LCOV_REPORT_DIR/coverage.info
mkdir $LCOV_REPORT_DIR/html
genhtml $LCOV_REPORT_DIR/coverage.info --output-directory $LCOV_REPORT_DIR/html
echo ==== lcov reports placed in $LCOV_REPORT_DIR

popd > /dev/null
echo ==== Coverage report script finished