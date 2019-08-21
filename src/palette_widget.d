import palette;
import utils;
import color_picker;

import gtk.DrawingArea;
import gtk.EventBox;
import gtk.Widget;
import gdk.Event;
import cairo.Context;
import cairo.ImageSurface;
import gobject.Signals;

class PaletteWidget : DrawingArea
{
    Palette* palette;
    ColorPicker colorPicker;
    const(double[3][3])* Minv;
    double gamma;
    ImageSurface surface;
    CairoOperator operator = CairoOperator.OVER;
    int width;
    int height;

    this(Palette* palette, const(double[3][3])* Minv, double gamma, ColorPicker colorPicker)
    {
        this.Minv = Minv;
        this.palette = palette;
        this.gamma = gamma;
        this.colorPicker = colorPicker;
        setSizeRequest(100, 50);
        addOnDraw(&onDraw);
        addOnSizeAllocate(&onSizeAllocate);
        this.addOnButtonPress(
            delegate bool(GdkEventButton* btn, Widget) {
                onButtonPress((*btn).x, (*btn).y);
                return true;
        });
    }

    void setGamma(double gamma)
    {
        this.gamma = gamma;
        drawPalette();
    }

    void setRGB(const double[3][3]* Minv)
    {
        this.Minv = Minv;
        drawPalette();
    }

    void drawPalette()
    {
        Context context = Context.create(surface);
        context.save();
        context.setOperator(operator);
        for (int x = 0; x < width; ++x) {
            double t = cast(double)x / width;
            RGB rgb = (*palette)(t).lchToLUV().luvToXYZ().xyzToRGB(*Minv, gamma);
            context.setSourceRgb(rgb.r, rgb.g, rgb.b);
            context.moveTo(x, 0);
            context.lineTo(x, height);
            context.stroke();
        }
        context.restore();
        this.queueDraw();
    }

private:
    void onButtonPress(double x, double y)
    {
        double t = cast(double)x / width;
        RGB rgb = (*palette)(t).lchToLUV().luvToXYZ().xyzToRGB(*Minv, gamma);
        colorPicker.setRGB(rgb);
    }

    void onSizeAllocate(GtkAllocation* allocation, Widget widget)
    {
        width = allocation.width;
        height = allocation.height;
        surface = ImageSurface.create(CairoFormat.ARGB32, width, height);
        drawPalette();
    }

    bool onDraw(Scoped!Context context, Widget widget)
    {
        context.setSourceSurface(surface, 0, 0);
        context.paint();
        return true;
    }
}
