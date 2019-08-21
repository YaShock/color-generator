import utils;
import color_picker;

import gtk.DrawingArea;
import gtk.Widget;
import cairo.Context;
import cairo.ImageSurface;

enum ColorFilterType
{
    LCH_L, LCH_C, LCH_H
}

class ColorSpaceWidget : DrawingArea
{
    ColorPicker colorPicker;
    ColorFilterType filterType = ColorFilterType.LCH_L;
    const(double[3][3])* Minv;
    double gamma = 2.2;
    double value = 0.0;
    ImageSurface surface;
    CairoOperator operator = CairoOperator.OVER;
    int width;
    int height;
    enum MIN_L = 0.0;
    enum MAX_L = 100.0;
    enum MIN_C = 0.0;
    enum MAX_C = 200.0;
    enum MIN_H = 0.0;
    enum MAX_H = 359.0;

    this(const double[3][3]* Minv, double gamma, ColorPicker colorPicker)
    {
        this.Minv = Minv;
        this.gamma = gamma;
        this.colorPicker = colorPicker;
        addOnDraw(&onDraw);
        addOnSizeAllocate(&onSizeAllocate);
        this.addOnButtonPress(
            delegate bool(GdkEventButton* btn, Widget) {
                onButtonPress((*btn).x, (*btn).y);
                return true;
        });
    }

    void setFilterType(ColorFilterType type)
    {
        this.filterType = type;
        drawSpace();
    }

    void setFilterValue(double value)
    {
        this.value = value;
        drawSpace();
    }

    void setGamma(double gamma)
    {
        this.gamma = gamma;
        drawSpace();
    }

    void setRGB(const double[3][3]* Minv)
    {
        this.Minv = Minv;
        drawSpace();
    }

private:
    void onButtonPress(double x, double y)
    {
        RGB rgb = getColor(x, y);
        colorPicker.setRGB(rgb);
    }

    void onSizeAllocate(GtkAllocation* allocation, Widget widget)
    {
        width = allocation.width;
        height = allocation.height;
        surface = ImageSurface.create(CairoFormat.ARGB32, width, height);
        drawSpace();
    }

    public bool onDraw(Scoped!Context context, Widget widget)
    {
        context.setSourceSurface(surface, 0, 0);
        context.paint();
        return true;
    }

    public void drawSpace()
    {
        Context context = Context.create(surface);
        context.save();
        context.setOperator(operator);
        context.setAntialias(cairo_antialias_t.NONE);

        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                RGB rgb = getColor(x, y);
                context.setSourceRgb(rgb.r, rgb.g, rgb.b);
                context.rectangle(x, y, 1, 1);
                context.fill();
            }
        }

        context.restore();
        this.queueDraw();
    }

    RGB getColor(double x, double y)
    {
        double sw, sh;
        double l, c, h;

        if (filterType == ColorFilterType.LCH_L) {
            l = value;
            sw = 200.0 / width;
            sh = 360.0 / height;
        }
        else if (filterType == ColorFilterType.LCH_C) {
            c = value;
            sw = 360.0 / width;
            sh = 100.0 / height;
        }
        else {
            h = value;
            sw = 200.0 / width;
            sh = 100.0 / height;
        }

        if (filterType == ColorFilterType.LCH_L) {
            c = sw * x;
            h = sh * y;
        }
        else if (filterType == ColorFilterType.LCH_C) {
            l = sh * y;
            h = sw * x;
        }
        else {
            l = sh * y;
            c = sw * x;
        }

        LCH lch = LCH(l, c, h);
        RGB rgb = lch.lchToLUV.luvToXYZ.xyzToRGB(*Minv, gamma);
        if (!rgb.isValid())
            rgb = RGB(0, 0, 0);
        return rgb;
    }
}
