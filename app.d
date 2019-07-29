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
import gtk.ComboBoxText;
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
    const double[3][3]* Minv;
    ImageSurface surface;
    Pixbuf pixbuf;
    CairoOperator operator = CairoOperator.OVER;
    int width;
    int height;

    this(Palette* palette, const double[3][3]* Minv)
    {
        this.Minv = Minv;
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
            RGB rgb = (*palette)(t).lchToLUV().luvToXYZ().xyzToRGB(*Minv, 2.2);
            context.setSourceRgb(rgb.r, rgb.g, rgb.b);
            context.moveTo(x, 0);
            context.lineTo(x, height);
            context.stroke();
        }
        context.restore();
        this.queueDraw();
    }
}

class Application {
    this()
    {
        initGui();
        setDefaultParams();
        palette = generatePalette(
            *M,
            gamma.getValue(),
            hue.getValue(),
            saturation.getValue(),
            brightness.getValue(),
            contrast.getValue());
    }

private:
    Scale hue, saturation, contrast, brightness;
    SpinButton numColors, gamma;
    ComboBoxText matrixType;
    Box boxPalette;
    PaletteWidget paletteWidget;
    Palette palette;
    const(double[3][3])* M = &M_SRGB;
    const(double[3][3])* Minv = &M_INV_SRGB;

    void initGui() {
        Builder builder = new Builder();
        builder.addFromFile("design.glade");
        Window w = cast(Window)builder.getObject("main-window");
        w.addOnHide( delegate void(Widget){ Main.quit(); } );
        w.showAll();

        Box boxMain = cast(Box)builder.getObject("box-main");

        paletteWidget = new PaletteWidget(&palette, Minv);
        paletteWidget.drawPalette();
        boxMain.packStart(paletteWidget, true, true, 5);

        boxPalette = new Box(Orientation.VERTICAL, 4);
        boxMain.add(boxPalette);
        boxMain.showAll();

        hue = cast(Scale)builder.getObject("scale-hue");
        saturation = cast(Scale)builder.getObject("scale-saturation");
        contrast = cast(Scale)builder.getObject("scale-contrast");
        brightness = cast(Scale)builder.getObject("scale-brightness");
        numColors = cast(SpinButton)builder.getObject("spin-num-colors");
        gamma = cast(SpinButton)builder.getObject("spin-gamma");
        matrixType = cast(ComboBoxText)builder.getObject("combo-matrix");

        hue.addOnValueChanged(delegate void(Range) {
            onClickedGenerate();
        });
        contrast.addOnValueChanged(delegate void(Range) {
            onClickedGenerate();
        });
        brightness.addOnValueChanged(delegate void(Range) {
            onClickedGenerate();
        });
        saturation.addOnValueChanged(delegate void(Range) {
            onClickedGenerate();
        });
        numColors.addOnValueChanged(delegate void(SpinButton button) {
            int N = to!int(button.getValue());
            double c = 0.88 < 0.34 + 0.06 * N ? 0.88 : 0.06 * N;
            contrast.setValue(c);
            onClickedGenerate();
        });
        gamma.addOnValueChanged(delegate void(SpinButton) {
            onClickedGenerate();
        });
        matrixType.addOnChanged(delegate void(ComboBoxText comboBox) {
            if (comboBox.getActiveText() == "SRGB") {
                M = &M_SRGB;
                Minv = &M_INV_SRGB;
            }
            else {
                M = &M_ADOBE;
                M = &M_INV_ADOBE;
            }
            onClickedGenerate();
        });
    }

    void onClickedGenerate()
    {
        int h = to!int(hue.getValue()) % 360;
        int N = to!int(numColors.getValue());
        palette = generatePalette(
            *M,
            gamma.getValue(),
            hue.getValue(),
            saturation.getValue(),
            brightness.getValue(),
            contrast.getValue());
        LCH[] colors = generateColors(palette, N);
        boxPalette.removeAll();
        foreach (lch; colors) {
            RGB rgb = lch.lchToLUV().luvToXYZ().xyzToRGB(*Minv, 2.2);
            Label label = new Label("");
            label.overrideBackgroundColor(GtkStateFlags.NORMAL, new RGBA(rgb.r, rgb.g, rgb.b));
            boxPalette.add(label);
        }
        paletteWidget.drawPalette();
        boxPalette.showAll();
    }

    void setDefaultParams()
    {
        int N = 8;
        double c = 0.88 < 0.34 + 0.06 * N ? 0.88 : 0.06 * N;
        hue.setValue(0);
        saturation.setValue(0.6);
        brightness.setValue(0.75);
        contrast.setValue(c);
        numColors.setValue(N);
        gamma.setValue(2.2);
    }
}


void main(string[] args)
{
    Main.init(args);
    Application app = new Application();
    Main.run();
}
