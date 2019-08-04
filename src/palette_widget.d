import palette;
import utils;

import gtk.DrawingArea;
import gtk.Widget;
import cairo.Context;
import cairo.ImageSurface;

class PaletteWidget : DrawingArea
{
    Palette* palette;
    const(double[3][3])* Minv;
    double gamma;
    ImageSurface surface;
    CairoOperator operator = CairoOperator.OVER;
    int width;
    int height;

    this(Palette* palette, const double[3][3]* Minv, double gamma)
    {
        this.Minv = Minv;
        this.palette = palette;
        this.gamma = gamma;
        setSizeRequest(100, 50);
        addOnDraw(&onDraw);
        addOnSizeAllocate(&onSizeAllocate);
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
            RGB rgb = (*palette)(t).lchToLUV().luvToXYZ().xyzToRGB(*Minv, gamma);
            context.setSourceRgb(rgb.r, rgb.g, rgb.b);
            context.moveTo(x, 0);
            context.lineTo(x, height);
            context.stroke();
        }
        context.restore();
        this.queueDraw();
    }
}
