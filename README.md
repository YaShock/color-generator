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

Just download and run the installer, no GTK runtime is required because it is bundled with the application.

## Building

You will need a D Compiler, dub, GTK runtime and gtkD.
For example DMD compiler can acquired from https://dlang.org/
To acquire gtkD follow the instructions at https://gtkd.org/

### Linux

You can simply run dub to fetch gtkd and build the application.

### Windows

gtkD has to be manually compiled, using dub produces errors. Use DMD to compile sources and link gtkd.

## Deploying

There are scripts for creating a windows installer and linux AppImage in /scripts. Modify them in order to generate the binaries.

### Linux

Preferred way is to use linuxdeploy and appimagetool.
Modify the script/linux/build_appimage.sh and change the path to these tools. Run the script and it will produce the AppImage.

### Windows

Copy the contents of GTK runtime to path/to/app/Gtk. Then use InnoSetup to generate the installer. You will need to change the paths accordingly.
