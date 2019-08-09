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
import gtk.Box;
import gtk.SpinButton;
import gtk.ComboBoxText;
import gtk.Adjustment;
import gtk.Widget;
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
    Scale hue, hue0, hue1, saturation, contrast, brightness, coldWarm;
    Scale midPoint, hueRange, hueShift;
    Label hueLabel, hue0Label, hue1Label, saturationLabel, contrastLabel;
    Label brightnessLabel, coldWarmLabel, midPointLabel, hueRangeLabel;
    Label hueShiftLabel;
    Widget[10] widgetsSeq;
    Widget[14] widgetsDiv;
    Widget[10] widgetsQual;
    SpinButton numColors, gamma, spaceFilterValue;
    ComboBoxText matrixType, spaceFilterType, pltType;
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

        hue = cast(Scale)builder.getObject("scale-hue");
        hue0 = cast(Scale)builder.getObject("scale-hue-0");
        hue1 = cast(Scale)builder.getObject("scale-hue-1");
        saturation = cast(Scale)builder.getObject("scale-saturation");
        contrast = cast(Scale)builder.getObject("scale-contrast");
        brightness = cast(Scale)builder.getObject("scale-brightness");
        coldWarm = cast(Scale)builder.getObject("scale-cold-warm");
        midPoint = cast(Scale)builder.getObject("scale-midpoint");
        hueRange = cast(Scale)builder.getObject("scale-range");
        hueShift = cast(Scale)builder.getObject("scale-shift");
        hueLabel = cast(Label)builder.getObject("label-hue");
        hue0Label = cast(Label)builder.getObject("label-hue-0");
        hue1Label = cast(Label)builder.getObject("label-hue-1");
        saturationLabel = cast(Label)builder.getObject("label-saturation");
        contrastLabel = cast(Label)builder.getObject("label-contrast");
        brightnessLabel = cast(Label)builder.getObject("label-brightness");
        coldWarmLabel = cast(Label)builder.getObject("label-cold-warm");
        midPointLabel = cast(Label)builder.getObject("label-midpoint");
        hueRangeLabel = cast(Label)builder.getObject("label-range");
        hueShiftLabel = cast(Label)builder.getObject("label-shift");
        numColors = cast(SpinButton)builder.getObject("spin-num-colors");
        gamma = cast(SpinButton)builder.getObject("spin-gamma");
        spaceFilterValue = cast(SpinButton)builder.getObject("spin_space_value");
        matrixType = cast(ComboBoxText)builder.getObject("combo-matrix");
        spaceFilterType = cast(ComboBoxText)builder.getObject("combo-space-filter");
        pltType = cast(ComboBoxText)builder.getObject("combo-palette-type");

        widgetsSeq = [hue, saturation, contrast, brightness, coldWarm,
                      hueLabel, saturationLabel, contrastLabel,
                      brightnessLabel, coldWarmLabel];
        widgetsDiv = [hue0, hue1, saturation, contrast, brightness, coldWarm,
                      midPoint, hue0Label, hue1Label, saturationLabel,
                      contrastLabel, brightnessLabel, coldWarmLabel,
                      midPointLabel];
        widgetsQual = [saturation, contrast, brightness, hueRange, hueShift,
                       saturationLabel, contrastLabel, brightnessLabel,
                       hueRangeLabel, hueShiftLabel];

        onSpaceFilterTypeChanged("LCH_L");
        onPaletteTypeChanged("Sequential");
    }

    void setCallbacks()
    {
        hue.addOnValueChanged(delegate void(Range) {
            updatePalette();
        });
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
        hueRange.addOnValueChanged(delegate void(Range) {
            updatePalette();
        });
        hueShift.addOnValueChanged(delegate void(Range) {
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

        matrixType.addOnChanged(delegate void(ComboBoxText comboBox) {
            onMatrixTypeChanged(comboBox.getActiveText());
        });
        spaceFilterType.addOnChanged(delegate void(ComboBoxText comboBox) {
            onSpaceFilterTypeChanged(comboBox.getActiveText());
        });
        spaceFilterValue.addOnValueChanged(delegate void(SpinButton button) {
            colorSpaceWidget.setFilterValue(spaceFilterValue.getValue());
        });
        pltType.addOnChanged(delegate void(ComboBoxText comboBox) {
            onPaletteTypeChanged(comboBox.getActiveText());
        });
    }

    void initGui()
    {
        loadWidgets;
        setCallbacks;
    }

    void onSpaceFilterTypeChanged(string filterType)
    {
        ColorFilterType type;
        if (filterType == "LCH_L") {
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
        else if (filterType == "LCH_C") {
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
        updatePalette();
    }

    void onMatrixTypeChanged(string type)
    {
        if (type == "SRGB") {
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
    }

    void onPaletteTypeChanged(string type)
    {
        // Deactivate all scales and labels
        foreach (widget; widgetsSeq) {
            if (widget.isVisible()) {
                widget.hide();
            }
        }
        foreach (widget; widgetsDiv) {
            if (widget.isVisible()) {
                widget.hide();
            }
        }
        foreach (widget; widgetsQual) {
            if (widget.isVisible()) {
                widget.hide();
            }
        }
         //Activate only current scales and labels
        if (type == "Sequential") {
            foreach (widget; widgetsSeq) {
                widget.show();
            }
        }
        else if (type == "Bivariate") {
            foreach (widget; widgetsDiv) {
                widget.show();
            }
        }
        else {
            foreach (widget; widgetsQual) {
                widget.show();
            }
        }
    }

    void updatePalette()
    {
        int h = to!int(hue.getValue) % 360;
        int h0 = to!int(hue0.getValue) % 360;
        int h1 = to!int(hue1.getValue) % 360;
        int N = to!int(numColors.getValue());

        if (pltType.getActiveText == "Sequential") {
            palette = generateSeqPalette(
                *M,
                gamma.getValue,
                h,
                saturation.getValue,
                brightness.getValue,
                contrast.getValue,
                coldWarm.getValue);
        }
        else if (pltType.getActiveText == "Bivariate") {
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
