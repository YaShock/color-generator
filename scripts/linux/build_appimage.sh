#!/bin/bash
rm -rf ../../AppDir
~/linuxdeploy-x86_64.AppImage --plugin gtk --appdir=../../AppDir -i ../../dist/icon.png -d color-generator.desktop -e ../../bin/color-generator
rm ../../bin/ColorGenerator-x86_64.AppImage
~/appimagetool-x86_64.AppImage ../../AppDir ../../bin/ColorGenerator-x86_64.AppImage
