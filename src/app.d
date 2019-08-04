import utils;
import palette;
import palette_widget;
import color_space_widget;

import gtk.Builder;
import gtk.Main;
import gtk.Window;
import gtk.Scale;
import gtk.Range;
import gtk.Label;
import gtk.Button;
import gtk.Box;
import gtk.CheckButton;
import gtk.ToggleButton;
import gtk.SpinButton;
import gtk.ComboBoxText;
import gtk.Adjustment;
import gdk.GLContext;
import gdk.RGBA;
import gobject.Signals;

import std.stdio;
import std.conv;
import std.string;
import std.algorithm;

class Application {
    this()
    {
        initGui;
        setDefaultParams;
        paletteWidget.setGamma(gamma.getValue);
        updatePalette();
    }

private:
    Scale hue0, hue1, saturation, contrast, brightness, coldWarm, midPoint;
    SpinButton numColors, gamma, spaceFilterValue;
    CheckButton useBivariate;
    ComboBoxText matrixType, spaceFilterType;
    Box boxPalette;
    PaletteWidget paletteWidget;
    ColorSpaceWidget colorSpaceWidget;
    Palette palette;
    const(double[3][3])* M = &M_SRGB;
    const(double[3][3])* Minv = &M_INV_SRGB;

    void loadWidgets() {
        Builder builder = new Builder();
        builder.addFromFile("design.glade");
        Window w = cast(Window)builder.getObject("main-window");
        w.addOnHide( delegate void(Widget){ Main.quit(); } );
        w.showAll();

        Box boxMain = cast(Box)builder.getObject("box-main");

        paletteWidget = new PaletteWidget(&palette, Minv, 2.2);
        paletteWidget.drawPalette();
        boxMain.packStart(paletteWidget, true, true, 5);

        boxPalette = new Box(Orientation.VERTICAL, 4);
        Box boxSpace = cast(Box)builder.getObject("box-space");

        boxMain.add(boxPalette);
        boxMain.showAll();

        colorSpaceWidget = new ColorSpaceWidget(Minv, 2.2);
        colorSpaceWidget.drawSpace();
        boxSpace.packEnd(colorSpaceWidget, true, true, 0);
        boxSpace.showAll();

        hue0 = cast(Scale)builder.getObject("scale-hue-0");
        hue1 = cast(Scale)builder.getObject("scale-hue-1");
        saturation = cast(Scale)builder.getObject("scale-saturation");
        contrast = cast(Scale)builder.getObject("scale-contrast");
        brightness = cast(Scale)builder.getObject("scale-brightness");
        coldWarm = cast(Scale)builder.getObject("scale-cold-warm");
        midPoint = cast(Scale)builder.getObject("scale-midpoint");
        numColors = cast(SpinButton)builder.getObject("spin-num-colors");
        gamma = cast(SpinButton)builder.getObject("spin-gamma");
        spaceFilterValue = cast(SpinButton)builder.getObject("spin_space_value");
        matrixType = cast(ComboBoxText)builder.getObject("combo-matrix");
        spaceFilterType = cast(ComboBoxText)builder.getObject("combo-space-filter");
        useBivariate = cast(CheckButton)builder.getObject("check-bivariate");

        spaceFilterValue.setAdjustment(
            new Adjustment(
                50.0,
                colorSpaceWidget.MIN_L,
                colorSpaceWidget.MAX_L,
                1.0,
                10.0,
                0.0));
    }

    void setCallbacks()
    {
        hue0.addOnValueChanged(delegate void(Range) {
            updatePalette();
        });
        hue1.addOnValueChanged(delegate void (Range) {
            updatePalette();
        });
        contrast.addOnValueChanged(delegate void(Range) {
            updatePalette();
        });
        brightness.addOnValueChanged(delegate void(Range) {
            updatePalette();
        });
        saturation.addOnValueChanged(delegate void(Range) {
            updatePalette();
        });
        coldWarm.addOnValueChanged(delegate void(Range) {
            updatePalette();
        });
        midPoint.addOnValueChanged(delegate void(Range) {
            updatePalette();
        });
        numColors.addOnValueChanged(delegate void(SpinButton button) {
            int N = to!int(button.getValue());
            double c = 0.88 < 0.34 + 0.06 * N ? 0.88 : 0.06 * N;
            contrast.setValue(c);
            updatePalette();
        });
        gamma.addOnValueChanged(delegate void(SpinButton button) {
            updatePalette();
            paletteWidget.setGamma(button.getValue());
            colorSpaceWidget.setGamma(button.getValue());
        });
        useBivariate.addOnToggled(delegate void(ToggleButton) {
            hue1.setSensitive(useBivariate.getActive);
            midPoint.setSensitive(useBivariate.getActive);
            updatePalette();
        });

        matrixType.addOnChanged(delegate void(ComboBoxText comboBox) {
            if (comboBox.getActiveText() == "SRGB") {
                M = &M_SRGB;
                Minv = &M_INV_SRGB;
            }
            else {
                M = &M_ADOBE;
                Minv = &M_INV_ADOBE;
            }
            paletteWidget.setRGB(Minv);
            colorSpaceWidget.setRGB(Minv);
            updatePalette();
        });
        spaceFilterType.addOnChanged(delegate void(ComboBoxText comboBox) {
            ColorFilterType type;
            if (comboBox.getActiveText() == "LCH_L") {
                type = type.LCH_L;
                spaceFilterValue.setAdjustment(
                    new Adjustment(
                        50.0,
                        colorSpaceWidget.MIN_L,
                        colorSpaceWidget.MAX_L,
                        1.0,
                        10.0,
                        0.0));
                spaceFilterValue.setValue(50);
            }
            else if (comboBox.getActiveText() == "LCH_C") {
                type = type.LCH_C;
                spaceFilterValue.setAdjustment(
                    new Adjustment(
                        0.0,
                        colorSpaceWidget.MIN_C,
                        colorSpaceWidget.MAX_C,
                        1.0,
                        10.0,
                        0.0));
                spaceFilterValue.setValue(0);
            }
            else {
                type = type.LCH_H;
                spaceFilterValue.setAdjustment(
                    new Adjustment(
                        0.0,
                        colorSpaceWidget.MIN_H,
                        colorSpaceWidget.MAX_H,
                        1.0,
                        10.0,
                        0.0));
                spaceFilterValue.setValue(0);
            }
            colorSpaceWidget.setFilterType(type);
        });
        spaceFilterValue.addOnValueChanged(delegate void(SpinButton button) {
            colorSpaceWidget.setFilterValue(spaceFilterValue.getValue());
        });
    }

    void initGui() {
        loadWidgets;
        setCallbacks;
    }

    void updatePalette()
    {
        int h0 = to!int(hue0.getValue) % 360;
        int h1 = to!int(hue1.getValue) % 360;
        int N = to!int(numColors.getValue());

        if (useBivariate.getActive) {
            palette = generateDivPalette(
                *M,
                gamma.getValue,
                h0,
                h1,
                saturation.getValue,
                brightness.getValue,
                contrast.getValue,
                coldWarm.getValue,
                midPoint.getValue);
        }
        else {
            palette = generateSeqPalette(
                *M,
                gamma.getValue,
                h0,
                saturation.getValue,
                brightness.getValue,
                contrast.getValue,
                coldWarm.getValue);
        }
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
        hue0.setValue(0);
        hue1.setValue(0);
        saturation.setValue(0.6);
        brightness.setValue(0.75);
        contrast.setValue(c);
        coldWarm.setValue(0.15);
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
