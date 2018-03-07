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
make INSTALL_ROOT=appdir install; find appdir/
wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt-continuous-x86_64.AppImage
unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
export VERSION=$(git rev-parse --short HEAD) # linuxdeployqt uses this for naming the file
./linuxdeployqt-continuous-x86_64.AppImage appdir/usr/share/applications/qlcplus.desktop -bundle-non-qt-libs
./linuxdeployqt-continuous-x86_64.AppImage appdir/usr/share/applications/qlcplus.desktop -appimage
exit $?
fi
