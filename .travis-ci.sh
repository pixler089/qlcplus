#!/bin/bash

# This script is triggered from the script section of .travis.yml
# It runs the appropriate commands depending on the task requested.

# Otherwise compile and check as normal
if [ "$TASK" = "compile" ]; then
$QMAKE && make && ./unittest.sh
exit $?
fi
if [ "$TASK" = "coverage" ]; then
$QMAKE CONFIG+=coverage && make && ./coverage.sh
exit $?
fi
if [ "$TASK" = "appimage" ]; then
$QMAKE CONFIG+=appimage CONFIG+=qmlui && make
exit $?
fi
