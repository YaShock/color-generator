import utils;

import gtk.Box;
import gtk.Label;
import gtk.SpinButton;
import gdk.RGBA;

import std.math;

class ColorPicker : Box
{
    this(const double[3][3]* M, const double[3][3]* Minv, double gamma)
    {
        super(GtkOrientation.HORIZONTAL, 0);
        this.M = M;
        this.Minv = Minv;
        this.gamma = gamma;
        color = new Label("");
        color.setSizeRequest(100, 100);
        this.packStart(color, true, true, 0);
        this.packStart(initHSV(), true, false, 0);
        this.packStart(initRGB(), true, false, 0);
        this.packStart(initXYZ(), true, false, 0);
        this.packStart(initLUV(), true, false, 0);
        this.packStart(initLCH(), true, false, 0);
        this.showAll();
        color.overrideBackgroundColor(
            GtkStateFlags.NORMAL,
            new RGBA(0.0, 0.0, 0.0));
    }

    void setGamma(double gamma)
    {
        this.gamma = gamma;
    }

    void setRGB(const double[3][3]* M, const double[3][3]* Minv)
    {
        this.M = M;
        this.Minv = Minv;
    }

    void setRGB(RGB rgb)
    {
        if (writing)
            return;

        writing = true;

        refRGB = rgb;
        refHSV = refRGB.rgbToHSV();
        refXYZ = rgb.rgbToXYZ(*M, gamma);
        refLUV = refXYZ.xyzToLUV();
        refLCH = refLUV.luvToLCH();

        setValues();
        writing = false;
    }

    void setHSV(HSV hsv)
    {
        if (writing)
            return;

        writing = true;

        refHSV = hsv;
        refRGB = hsv.hsvToRGB();
        refXYZ = refRGB.rgbToXYZ(*M, gamma);
        refLUV = refXYZ.xyzToLUV();
        refLCH = refLUV.luvToLCH();

        setValues();

        writing = false;
    }

    void setXYZ(XYZ xyz)
    {
        if (writing)
            return;

        writing = true;

        refXYZ = xyz;
        refRGB = xyz.xyzToRGB(*Minv, gamma);
        refHSV = refRGB.rgbToHSV();
        refLUV = xyz.xyzToLUV();
        refLCH = refLUV.luvToLCH();

        setValues();
        writing = false;
    }

    void setLUV(LUV luv)
    {
        if (writing)
            return;

        writing = true;

        refLUV = luv;
        refLCH = luv.luvToLCH();
        refXYZ = luv.luvToXYZ;
        refRGB = refXYZ.xyzToRGB(*Minv, gamma);
        refHSV = refRGB.rgbToHSV();

        setValues();
        writing = false;
    }

    void setLCH(LCH lch)
    {
        if (writing)
            return;

        writing = true;

        refLCH = lch;
        refLUV = lch.lchToLUV();
        refXYZ = refLUV.luvToXYZ();
        refRGB = refXYZ.xyzToRGB(*Minv, gamma);
        refHSV = refRGB.rgbToHSV();

        setValues();
        writing = false;
    }

private:
    void updateRGB() {
        setRGB(RGB(
            red.getValue(),
            green.getValue(),
            blue.getValue()));
    }

    void updateHSV() {
        setHSV(HSV(
            hue.getValue(),
            sat.getValue(),
            val.getValue()));
    }

    void updateXYZ() {
        setXYZ(XYZ(
            xyzX.getValue(),
            xyzY.getValue(),
            xyzZ.getValue()));
    }

    void updateLUV() {
        setLUV(LUV(
            luvL.getValue(),
            luvU.getValue(),
            luvV.getValue()));
    }

    void updateLCH() {
        setLCH(LCH(
            lchL.getValue(),
            lchC.getValue(),
            lchH.getValue()));
    }

    void setValues()
    {
        color.overrideBackgroundColor(
            GtkStateFlags.NORMAL,
            new RGBA(refRGB.r, refRGB.g, refRGB.b));

        red.setValue(refRGB.r);
        green.setValue(refRGB.g);
        blue.setValue(refRGB.b);

        xyzX.setValue(refXYZ.X);
        xyzY.setValue(refXYZ.Y);
        xyzZ.setValue(refXYZ.Z);

        luvL.setValue(refLUV.L);
        luvU.setValue(refLUV.U);
        luvV.setValue(refLUV.V);

        lchL.setValue(refLCH.L);
        lchC.setValue(refLCH.C);
        lchH.setValue(refLCH.H);

        hue.setValue(refHSV.H);
        sat.setValue(refHSV.S);
        val.setValue(refHSV.V);
    }

    Box initHSV()
    {
        Box hsv = new Box(GtkOrientation.VERTICAL, 0);
        Label labelHue = new Label("Hue");
        Label labelSat = new Label("Saturation");
        Label labelVal = new Label("Value");
        hue = new SpinButton(0, 359, 1);
        sat = new SpinButton(0, 1, 0.01);
        val = new SpinButton(0, 1, 0.01);
        auto boxHue = new Box(GtkOrientation.HORIZONTAL, 0);
        auto boxSat = new Box(GtkOrientation.HORIZONTAL, 0);
        auto boxVal = new Box(GtkOrientation.HORIZONTAL, 0);
        boxHue.packStart(labelHue, true, false, 0);
        boxHue.packStart(hue, true, false, 0);
        boxSat.packStart(labelSat, true, false, 0);
        boxSat.packStart(sat, true, false, 0);
        boxVal.packStart(labelVal, true, false, 0);
        boxVal.packStart(val, true, false, 0);
        hsv.packStart(boxHue, true, false, 0);
        hsv.packStart(boxSat, true, false, 0);
        hsv.packStart(boxVal, true, false, 0);
        labelHue.setHalign(GtkAlign.START);
        labelSat.setHalign(GtkAlign.START);
        labelVal.setHalign(GtkAlign.START);

        hue.addOnValueChanged(delegate void(SpinButton) {
            updateHSV();
        });
        sat.addOnValueChanged(delegate void(SpinButton) {
            updateHSV();
        });
        val.addOnValueChanged(delegate void(SpinButton) {
            updateHSV();
        });

        return hsv;
    }

    Box initRGB()
    {
        Box rgb = new Box(GtkOrientation.VERTICAL, 0);
        Label labelRed = new Label("Red");
        Label labelGreen = new Label("Green");
        Label labelBlue = new Label("Blue");
        red = new SpinButton(0, 1, 0.01);
        green = new SpinButton(0, 1, 0.01);
        blue = new SpinButton(0, 1, 0.01);
        auto boxRed = new Box(GtkOrientation.HORIZONTAL, 0);
        auto boxGreen = new Box(GtkOrientation.HORIZONTAL, 0);
        auto boxBlue = new Box(GtkOrientation.HORIZONTAL, 0);
        boxRed.packStart(labelRed, true, false, 0);
        boxRed.packStart(red, true, false, 0);
        boxGreen.packStart(labelGreen, true, false, 0);
        boxGreen.packStart(green, true, false, 0);
        boxBlue.packStart(labelBlue, true, false, 0);
        boxBlue.packStart(blue, true, false, 0);
        labelRed.setHalign(GtkAlign.START);
        labelGreen.setHalign(GtkAlign.START);
        labelBlue.setHalign(GtkAlign.START);
        rgb.packStart(boxRed, true, false, 0);
        rgb.packStart(boxGreen, true, false, 0);
        rgb.packStart(boxBlue, true, false, 0);

        red.addOnValueChanged(delegate void(SpinButton) {
            updateRGB();
        });
        green.addOnValueChanged(delegate void(SpinButton) {
            updateRGB();
        });
        blue.addOnValueChanged(delegate void(SpinButton) {
            updateRGB();
        });

        return rgb;
    }

    Box initXYZ()
    {
        Box xyz = new Box(GtkOrientation.VERTICAL, 0);
        Label labelX = new Label("X");
        Label labelY = new Label("Y");
        Label labelZ = new Label("Z");
        xyzX = new SpinButton(0, 2, 0.01);
        xyzY = new SpinButton(0, 2, 0.01);
        xyzZ = new SpinButton(0, 2, 0.01);
        auto boxX = new Box(GtkOrientation.HORIZONTAL, 0);
        auto boxY = new Box(GtkOrientation.HORIZONTAL, 0);
        auto boxZ = new Box(GtkOrientation.HORIZONTAL, 0);
        boxX.packStart(labelX, true, false, 0);
        boxX.packStart(xyzX, true, false, 0);
        boxY.packStart(labelY, true, false, 0);
        boxY.packStart(xyzY, true, false, 0);
        boxZ.packStart(labelZ, true, false, 0);
        boxZ.packStart(xyzZ, true, false, 0);
        labelX.setHalign(GtkAlign.START);
        labelY.setHalign(GtkAlign.START);
        labelZ.setHalign(GtkAlign.START);
        xyz.packStart(boxX, true, false, 0);
        xyz.packStart(boxY, true, false, 0);
        xyz.packStart(boxZ, true, false, 0);

        xyzX.addOnValueChanged(delegate void(SpinButton) {
            updateXYZ();
        });
        xyzY.addOnValueChanged(delegate void(SpinButton) {
            updateXYZ();
        });
        xyzZ.addOnValueChanged(delegate void(SpinButton) {
            updateXYZ();
        });

        return xyz;
    }

    Box initLUV()
    {
        Box luv = new Box(GtkOrientation.VERTICAL, 0);
        Label labelL = new Label("L");
        Label labelU = new Label("U");
        Label labelV = new Label("V");
        luvL = new SpinButton(0, 100, 1);
        luvU = new SpinButton(-200, 200, 1);
        luvV = new SpinButton(-200, 200, 1);
        auto boxL = new Box(GtkOrientation.HORIZONTAL, 0);
        auto boxU = new Box(GtkOrientation.HORIZONTAL, 0);
        auto boxV = new Box(GtkOrientation.HORIZONTAL, 0);
        boxL.packStart(labelL, true, false, 0);
        boxL.packStart(luvL, true, false, 0);
        boxU.packStart(labelU, true, false, 0);
        boxU.packStart(luvU, true, false, 0);
        boxV.packStart(labelV, true, false, 0);
        boxV.packStart(luvV, true, false, 0);
        labelL.setHalign(GtkAlign.START);
        labelU.setHalign(GtkAlign.START);
        labelV.setHalign(GtkAlign.START);
        luv.packStart(boxL, true, false, 0);
        luv.packStart(boxU, true, false, 0);
        luv.packStart(boxV, true, false, 0);

        luvL.addOnValueChanged(delegate void(SpinButton) {
            updateLUV();
        });
        luvU.addOnValueChanged(delegate void(SpinButton) {
            updateLUV();
        });
        luvV.addOnValueChanged(delegate void(SpinButton) {
            updateLUV();
        });

        return luv;
    }

    Box initLCH()
    {
        Box lch = new Box(GtkOrientation.VERTICAL, 0);
        Label labelL = new Label("L");
        Label labelC = new Label("C");
        Label labelH = new Label("H");
        lchL = new SpinButton(0, 100, 1);
        lchC = new SpinButton(0, 200, 1);
        lchH = new SpinButton(0, 359, 1);
        auto boxL = new Box(GtkOrientation.HORIZONTAL, 0);
        auto boxC = new Box(GtkOrientation.HORIZONTAL, 0);
        auto boxH = new Box(GtkOrientation.HORIZONTAL, 0);
        boxL.packStart(labelL, true, false, 0);
        boxL.packStart(lchL, true, false, 0);
        boxC.packStart(labelC, true, false, 0);
        boxC.packStart(lchC, true, false, 0);
        boxH.packStart(labelH, true, false, 0);
        boxH.packStart(lchH, true, false, 0);
        labelL.setHalign(GtkAlign.START);
        labelC.setHalign(GtkAlign.START);
        labelH.setHalign(GtkAlign.START);
        lch.packStart(boxL, true, false, 0);
        lch.packStart(boxC, true, false, 0);
        lch.packStart(boxH, true, false, 0);

        lchL.addOnValueChanged(delegate void(SpinButton) {
            updateLCH();
        });
        lchC.addOnValueChanged(delegate void(SpinButton) {
            updateLCH();
        });
        lchH.addOnValueChanged(delegate void(SpinButton) {
            updateLCH();
        });

        return lch;
    }

    Label color;
    SpinButton hue, sat, val;
    SpinButton red, green, blue;
    SpinButton xyzX, xyzY, xyzZ;
    SpinButton lchL, lchC, lchH;
    SpinButton luvL, luvU, luvV;
    const(double[3][3])* M;
    const(double[3][3])* Minv;
    double gamma = 0.0;

    bool writing = false;
    HSV refHSV;
    RGB refRGB;
    XYZ refXYZ;
    LUV refLUV;
    LCH refLCH;
}
