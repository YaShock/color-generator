#!/bin/bash
rm -rf ../../AppDir
~/linuxdeploy-x86_64.AppImage --appdir=../../AppDir -i ../../dist/icon.png -d color-generator.desktop -e ../../color-generator
cp ../../design.glade ../../AppDir/usr/bin
~/appimagetool-x86_64.AppImage ../../AppDir
