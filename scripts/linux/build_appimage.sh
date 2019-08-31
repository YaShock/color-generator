#!/bin/bash
rm -rf ../../AppDir
~/linuxdeploy-x86_64.AppImage --appdir=../../AppDir -i ../../dist/icon.png -d color-generator.desktop -e ../../color-generator
cp ../../design.glade ../../AppDir/usr/bin
mkdir ../../bin
rm ../../bin/ColorGenerator-x86_64.AppImage
~/appimagetool-x86_64.AppImage ../../AppDir ../../bin/ColorGenerator-x86_64.AppImage
