# Color Generator

An application that can generate perceptually uniforms palettes using CIE LCH(uv) space.
It is based on the paper "Synthesis of color palettes" by By MWH Martijn Wijffelaars.

There are three palette generation methods
 - Sequatial
 - Bivariate
 - Qualitative

All generation methods are controlled with a set of intuitive parameters. To generate such palettes the work is done in CIE LCH(uv) color space which is then converted back to RGB. For further details read the paper.

The application also includes a color picker which can be used as a converter between different color spaces, and a visualization of a given color space.

## Installing

Binaries are provided for Linux via AppImage and Windows via an installer.

### Linux

Requires a GTK runtime. If installed, just download the AppImage and make it runnable:
`chmod a+x ColorGenerator-x86_64.AppImage`

### Windows

Just download and run the installer, no runtime is required because it is bundled with the application.

## Building

You will need a C++ compiler, CMake and wxWidgets.

### Linux

Simply run CMake to generate a Makefile and then use make to build. If you installed wxWidgets through a package manager, then CMake will automatically find wxWidgets through _find_package_ macro.

### Windows

The wxWigets libary has to be installed or manually build (I prefer the latter case). Using CMake GUI you can set the wxWidgets path after running _configure_.

Mac OS

#### TODO

## Deploying

There are scripts for creating a windows installer and linux AppImage in /scripts. Modify them in order to generate the binaries.

### Linux

Preferred way is to use linuxdeploy and appimagetool.
Modify the script/linux/build_appimage.sh and change the path to these tools. Run the script and it will produce the AppImage.

### Windows

Install InnoSetup in order to create installer for the application. You should be able to just compile the script found in _scripts/windows_, which produces a Windows installer.

### Mac OS

TODO
