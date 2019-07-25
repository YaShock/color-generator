pragma(lib, "gtkd-3");

import utils;

import gtk.Builder;
import gtk.Main;
import gtk.Window;
import gtk.Scale;
import gtk.Range;
import gtk.Label;
import gtk.Button;
import gtk.Box;
import gtk.SpinButton;
import gtk.DrawingArea;
import gdk.GLContext;
import gdk.RGBA;
import gobject.Signals;

import std.stdio;
import std.conv;
import std.string;

class PaletteWidget : DrawingArea
{
    this()
    {
        this.setSizeRequest(100, 50);
    }
}

void onClickedGenerate(ref Box boxPalette, int numColors, double hue, double s, double b, double c)
{
    hue = hue % 360;
    writeln("Generate with hue ", hue);
    LCH[] colors = generatePalette(numColors, hue, s, b, c);
    boxPalette.removeAll();
    foreach (lch; colors) {
        writeln("LCH ", lch.L, ' ', lch.C, ' ', lch.H);
        RGB rgb = lch.lchToLUV().luvToXYZ().xyzToRGB(2.2);
        Label label = new Label("");
        label.overrideBackgroundColor(GtkStateFlags.NORMAL, new RGBA(rgb.r, rgb.g, rgb.b));
        boxPalette.add(label);
        writeln("RGB ", rgb.r, ' ', rgb.g, ' ', rgb.b);
    }
    boxPalette.showAll();
}

void setDefaultParams(int numColors, ref double hue, ref double s, ref double b, ref double c)
{
    hue = 0;
    s = 0.6;
    b = 0.75;
    c = 0.88 < 0.34 + 0.06 * numColors ? 0.88 : 0.06 * numColors;
}

void main(string[] args)
{
    int numColors = 8;
    double hue, s, b, c;
    setDefaultParams(numColors, hue, s, b, c);

    Main.init(args);
    Builder builder = new Builder();
    builder.addFromFile("design.glade");
    Window w = cast(Window)builder.getObject("main-window");
    w.addOnHide( delegate void(Widget){ Main.quit(); } );
    w.showAll();

    Box boxMain = cast(Box)builder.getObject("box-main");

    PaletteWidget paletteWidget = new PaletteWidget();
    boxMain.add(paletteWidget);

    Box boxPalette = new Box(Orientation.VERTICAL, 4);
    boxMain.add(boxPalette);
    boxMain.showAll();

    Scale scaleHue = cast(Scale)builder.getObject("scale-hue");
    Scale scaleSaturation = cast(Scale)builder.getObject("scale-saturation");
    Scale scaleContrast = cast(Scale)builder.getObject("scale-contrast");
    Scale scaleBrightness = cast(Scale)builder.getObject("scale-brightness");

    scaleHue.addOnValueChanged(delegate void(Range range) {
        hue = range.getValue();
        onClickedGenerate(boxPalette, numColors, hue, s, b, c);
    });

    scaleContrast.addOnValueChanged(delegate void(Range range) {
        c = range.getValue();
        onClickedGenerate(boxPalette, numColors, hue, s, b, c);
    });

    scaleBrightness.addOnValueChanged(delegate void(Range range) {
        b = range.getValue();
        onClickedGenerate(boxPalette, numColors, hue, s, b, c);
    });

    scaleSaturation.addOnValueChanged(delegate void(Range range) {
        s = range.getValue();
        onClickedGenerate(boxPalette, numColors, hue, s, b, c);
    });

    scaleHue.setValue(hue);
    scaleSaturation.setValue(s);
    scaleContrast.setValue(c);
    scaleBrightness.setValue(b);

    SpinButton spinNumColors = cast(SpinButton)builder.getObject("spin-num-colors");
    spinNumColors.setValue(numColors);
    spinNumColors.addOnValueChanged(delegate void(SpinButton button) {
        numColors = to!int(button.getValue());
        c = 0.88 < 0.34 + 0.06 * numColors ? 0.88 : 0.06 * numColors;
        scaleContrast.setValue(c);
        onClickedGenerate(boxPalette, numColors, hue, s, b, c);
    });

    Main.run();
}
