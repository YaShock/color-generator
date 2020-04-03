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

Just download the AppImage and make it executable:
`chmod a+x ColorGenerator-x86_64.AppImage`

### Windows

Just download and run the installer, no runtime is required because it is bundled with the application.

### MacOS

TODO

## Building

You will need a C++ compiler, CMake and wxWidgets.

### Linux

Simply run CMake to generate a Makefile and then use make to build. If you installed wxWidgets through a package manager, then CMake will automatically find wxWidgets through _find_package_ macro.

### Windows

The wxWidgets library has to be installed or manually build (I prefer the latter case). Using CMake GUI you can set the wxWidgets path after running _configure_.

### MacOS

Download and build the wxWidgets library. Ater that you can use CMake to generate Unix Makefiles. If CMake fails to find wxWidgets, configure wxWidgets_CONFIG_EXECUTABLE flag to wx-config executable. It should be under the build directory in wxWidgets root. After generating Makefile just run _make_.

## Deploying

There are scripts for creating a windows installer and linux AppImage in /scripts. Modify them in order to generate the binaries.

### Linux

Preferred way is to use linuxdeploy and appimagetool.
Modify the script/linux/build_appimage.sh and change the path to these tools. Run the script from the _scripts/linux_ directory and it will produce the AppImage in _bin_.

### Windows

Install InnoSetup in order to create installer for the application. You should be able to just compile the script found in _scripts/windows_, which produces a Windows installer.

### MacOS

TODO
