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
import gtk.Widget;
import gdk.GLContext;
import gdk.RGBA;
import gdk.Pixbuf;
import gobject.Signals;
import cairo.Context;
import cairo.ImageSurface;

import std.stdio;
import std.conv;
import std.string;
import std.algorithm;

class PaletteWidget : DrawingArea
{
    Palette* palette;
    ImageSurface surface;
    Pixbuf pixbuf;
    CairoOperator operator = CairoOperator.OVER;
    int width;
    int height;

    this(Palette* palette)
    {
        this.palette = palette;
        setSizeRequest(100, 50);
        addOnDraw(&onDraw);
        addOnSizeAllocate(&onSizeAllocate);
    }

    void onSizeAllocate(GtkAllocation* allocation, Widget widget)
    {
        width = allocation.width;
        height = allocation.height;
        surface = ImageSurface.create(CairoFormat.ARGB32, width, height);
        drawPalette();
    }

    public bool onDraw(Scoped!Context context, Widget widget)
    {
        context.setSourceSurface(surface, 0, 0);
        context.paint();
        return true;
    }

    public void drawPalette()
    {
        Context context = Context.create(surface);
        context.save();
        context.setOperator(operator);
        for (int x = 0; x < width; ++x) {
            double t = cast(double)x / width;
            RGB rgb = (*palette)(t).lchToLUV().luvToXYZ().xyzToRGB(2.2);
            context.setSourceRgb(rgb.r, rgb.g, rgb.b);
            context.moveTo(x, 0);
            context.lineTo(x, height);
            context.stroke();
        }
        context.restore();
        this.queueDraw();
    }
}

void onClickedGenerate(
    ref PaletteWidget paletteWidget,
    ref Box boxPalette,
    ref Palette palette,
    int numColors,
    double hue,
    double s,
    double b,
    double c)
{
    hue = hue % 360;
    palette = generatePalette(hue, s, b, c);
    LCH[] colors = generateColors(palette, numColors);
    boxPalette.removeAll();
    foreach (lch; colors) {
        RGB rgb = lch.lchToLUV().luvToXYZ().xyzToRGB(2.2);
        Label label = new Label("");
        label.overrideBackgroundColor(GtkStateFlags.NORMAL, new RGBA(rgb.r, rgb.g, rgb.b));
        boxPalette.add(label);
    }
    paletteWidget.drawPalette();
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
    Palette palette = generatePalette(hue, s, b, c);

    Main.init(args);
    Builder builder = new Builder();
    builder.addFromFile("design.glade");
    Window w = cast(Window)builder.getObject("main-window");
    w.addOnHide( delegate void(Widget){ Main.quit(); } );
    w.showAll();

    Box boxMain = cast(Box)builder.getObject("box-main");

    PaletteWidget paletteWidget = new PaletteWidget(&palette);
    paletteWidget.drawPalette();
    boxMain.packStart(paletteWidget, true, true, 5);

    Box boxPalette = new Box(Orientation.VERTICAL, 4);
    boxMain.add(boxPalette);
    boxMain.showAll();

    Scale scaleHue = cast(Scale)builder.getObject("scale-hue");
    Scale scaleSaturation = cast(Scale)builder.getObject("scale-saturation");
    Scale scaleContrast = cast(Scale)builder.getObject("scale-contrast");
    Scale scaleBrightness = cast(Scale)builder.getObject("scale-brightness");

    scaleHue.addOnValueChanged(delegate void(Range range) {
        hue = range.getValue();
        onClickedGenerate(paletteWidget, boxPalette, palette, numColors, hue, s, b, c);
    });

    scaleContrast.addOnValueChanged(delegate void(Range range) {
        c = range.getValue();
        onClickedGenerate(paletteWidget, boxPalette, palette, numColors, hue, s, b, c);
    });

    scaleBrightness.addOnValueChanged(delegate void(Range range) {
        b = range.getValue();
        onClickedGenerate(paletteWidget, boxPalette, palette, numColors, hue, s, b, c);
    });

    scaleSaturation.addOnValueChanged(delegate void(Range range) {
        s = range.getValue();
        onClickedGenerate(paletteWidget, boxPalette, palette, numColors, hue, s, b, c);
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
        onClickedGenerate(paletteWidget, boxPalette, palette, numColors, hue, s, b, c);
    });

    Main.run();
}
