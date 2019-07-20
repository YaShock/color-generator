pragma(lib, "gtkd-3");

import gtk.Builder;
import gtk.Main;
import gtk.Window;
import gtk.Scale;
import gtk.Range;
import gtk.Label;
import gtk.Button;
import gtk.Box;
import gtk.SpinButton;
import gdk.RGBA;
import gobject.Signals;

import std.stdio;
import std.math;
import std.conv;
import std.string;

struct RGB {
    double r, g, b;
    ref double opIndex(int i) {
        if (i == 0)
            return r;
        if (i == 1)
            return g;
        if (i == 2)
            return b;
        throw new Exception("Invalid index");
    }

    RGB opMul(double s) const {
        return RGB(s*r, s*g, s*b);
    }
};

struct XYZ {
    double X, Y, Z;

    ref double opIndex(int i) {
        if (i == 0)
            return X;
        if (i == 1)
            return Y;
        if (i == 2)
            return Z;
        throw new Exception("Invalid index");
    }
};

struct LUV {
    double L, U, V;
};

struct LCH {
    double L, C, H;

    LCH opMul(double s) const {
        return LCH(s*L, s*C, s*H);
    }

    LCH opAdd(LCH rhs) const {
        return LCH(L + rhs.L, C + rhs.C, H + rhs.H);
    }
};

XYZ rgbToXYZ(RGB rgb, double gamma) {
    double[][] M = [
        [0.4124564, 0.3575761, 0.1804375],
        [0.2126729, 0.7151522, 0.0721750],
        [0.0193339, 0.1191920, 0.9503041]
    ];
    double[] xyz = [0, 0, 0];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            xyz[i] += pow(rgb[j], gamma) * M[i][j];
        }
    }
    return XYZ(xyz[0], xyz[1], xyz[2]);
}

RGB xyzToRGB(XYZ xyz, double gamma) {
    double[][] Minv = [
        [3.2404542, -1.5371385, -0.4985314],
        [-0.9692660, 1.8760108, 0.0415560],
        [0.0556434, -0.2040259, 1.0572252]
    ];
    double[] rgb = [0, 0, 0];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rgb[i] += pow(xyz[j], 1/gamma) * Minv[i][j];
        }
    }
    return RGB(rgb[0], rgb[1], rgb[2]);
}

LUV xyzToLUV(XYZ xyz) {
    const double Xn = 0.95047;
    const double Yn = 1.0;
    const double Zn = 1.08883;
    const double eps = 0.008856;
    const double ki = 903.3;
    double U = 4*xyz.X/(xyz.X+15*xyz.Y+3*xyz.Z);
    double V = 9*xyz.Y/(xyz.X+15*xyz.Y+3*xyz.Z);
    double Un = 4*Xn/(Xn+15*Yn+3*Zn);
    double Vn = 9*Yn/(Xn+15*Yn+3*Zn);
    double yn = xyz.Y/Yn;
    double L;
    if (yn > eps)
        L = 116*pow(yn, 1.0/3)-16;
    else
        L = ki*yn;
    return LUV(L, 13*L*(U-Un), 13*L*(V-Vn));
}

XYZ luvToXYZ(LUV luv) {
    const double Xn = 0.95047;
    const double Yn = 1.0;
    const double Zn = 1.08883;
    const double eps = 0.008856;
    const double ki = 903.3;
    const double L = luv.L;

    const double u0 = 4*Xn/(Xn + 15*Yn + 3*Zn);
    const double v0 = 9*Yn/(Xn + 15*Yn + 3*Zn);
    const double Y = L > ki*eps ? pow(((L + 16) / 116.0), 3.0) : L / ki;

    const double a = L > 0 ? 1.0/3.0*(52*L/(luv.U + 13*L*u0)-1) : 0;
    const double b = -5*Y;
    const double c = -1.0/3.0;
    const double d = luv.V + 13*L*v0 > 0 ? Y*(39*L/(luv.V + 13*L*v0)-5) : 0;

    const double X = (d - b) / (a - c);
    const double Z = X*a + b;

    return XYZ(X, Y, Z);
}

LCH luvToLCH(LUV luv) {
    double H;
    if (atan2(luv.V, luv.U) >= 0)
        H = atan2(luv.V, luv.U)*180.0/PI;
    else
        H = atan2(luv.V, luv.U)*180.0/PI + 360;
    return LCH(luv.L, sqrt(luv.U*luv.U + luv.V*luv.V), H);
}

LUV lchToLUV(LCH lch) {
    const double C = lch.C;
    const double H = lch.H;
    return LUV(lch.L, C * cos(H * PI / 180), C * sin(H * PI / 180));
}

RGB MSC(double hue)
{
    const double[][] M = [
        [0.4124564, 0.3575761, 0.1804375],
        [0.2126729, 0.7151522, 0.0721750],
        [0.0193339, 0.1191920, 0.9503041]
    ];
    const double Xn = 0.95047;
    const double Yn = 1.0;
    const double Zn = 1.08883;
    int ro, sigma, theta;
    const double hRed = luvToLCH(xyzToLUV(rgbToXYZ(RGB(1.0, 0.0, 0.0), 2.2))).H;
    const double hYellow = luvToLCH(xyzToLUV(rgbToXYZ(RGB(1.0, 1.0, 0.0), 2.2))).H;
    const double hGreen = luvToLCH(xyzToLUV(rgbToXYZ(RGB(0.0, 1.0, 0.0), 2.2))).H;
    const double hCyan = luvToLCH(xyzToLUV(rgbToXYZ(RGB(0.0, 1.0, 1.0), 2.2))).H;
    const double hBlue = luvToLCH(xyzToLUV(rgbToXYZ(RGB(0.0, 0.0, 1.0), 2.2))).H;
    const double hMagenta = luvToLCH(xyzToLUV(rgbToXYZ(RGB(1.0, 0.0, 1.0), 2.2))).H;
    writeln("hRed ", hRed);
    writeln("hYellow ", hYellow);
    writeln("hGreen ", hGreen);
    writeln("hCyan ", hCyan);
    writeln("hBlue ", hBlue);
    writeln("hMagenta ", hMagenta);
    if (hue >= hRed && hue <= hYellow) {
        ro = 1;
        sigma = 2;
        theta = 0;
    }
    else if (hue > hYellow && hue <= hGreen) {
        ro = 0;
        sigma = 2;
        theta = 1;
    }
    else if (hue > hGreen && hue <= hCyan) {
        ro = 2;
        sigma = 0;
        theta = 1;
    }
    else if (hue > hCyan && hue <= hBlue) {
        ro = 1;
        sigma = 0;
        theta = 2;
    }
    else if (hue > hBlue && hue <= hMagenta) {
        ro = 0;
        sigma = 1;
        theta = 2;
    }
    else {
        ro = 2;
        sigma = 1;
        theta = 0;
    }

    const double Un = 4*Xn/(Xn+15*Yn+3*Zn);
    const double Vn = 9*Yn/(Xn+15*Yn+3*Zn);
    const double gamma = 2.2;
    const double alpha = -sin(hue * PI / 180);
    const double beta = cos(hue * PI / 180);
    const double a = (alpha*Un+beta*Vn)*(M[0][theta] + 15*M[1][theta] + 3*M[2][theta])-(4*alpha*M[0][theta]+9*beta*M[1][theta]);
    const double b = (alpha*Un+beta*Vn)*(M[0][ro] + 15*M[1][ro] + 3*M[2][ro])-(4*alpha*M[0][ro]+9*beta*M[1][ro]);
    const double roValue = pow(-a/b, 1.0/gamma);

    auto color = RGB(0, 0, 0);
    color[sigma] = 0;
    color[theta] = 1;
    color[ro] = roValue;
    return color;
}

LCH lchBezier(LCH b0, LCH b1, LCH b2, double t)
{
    return (t - 1)*(t - 1)*b0 + 2*(1 - t)*t*b1 + t*t*b2;
}

struct BezierLCH {
    LCH b0;
    LCH b1;
    LCH b2;

    this(LCH b0, LCH b1, LCH b2) {
        this.b0 = b0;
        this.b1 = b1;
        this.b2 = b2;
    }

    LCH opCall(double t) const {
        return (t - 1)*(t - 1)*b0 + 2*(1 - t)*t*b1 + t*t*b2;
    }
};

double distance(LCH c1, LCH c2) {
    return abs(log((125 - c2.L) / (125 - c1.L)));
}

LCH[] generatePalette(int numColors, double hue, double s, double b, double c)
{
    LCH[] generated = new LCH[numColors];

    const int N = numColors;
    const LCH p0 = LCH(0.0, 0.0, hue);
    const RGB msc = MSC(hue);
    const LCH p1 = luvToLCH(xyzToLUV(rgbToXYZ(msc, 2.2)));
    const LCH p2 = LCH(100.0, 0.0, hue);
    //writeln("MSC(RGB) ", msc.r, ' ', msc.g, ' ', msc.b);
    //writeln("MSC(LCH) ", p1.L, ' ', p1.C, ' ', p1.H);
    const LCH q0 = (1 - s)*p0 + s*p1;
    const LCH q2 = (1 - s)*p2 + s*p1;
    const LCH q1 = 1.0/2.0*(q0 + q2);
    auto B0 = BezierLCH(p0, q0, q1);
    auto B1 = BezierLCH(q1, q2, p2);
    const int K = N - 1;

    LCH cSeq(double t) {
        if (t <= 0.5)
            return B0(2*t);
        else
            return B1(2*(t-0.5));
    }

    LCH pSeq(double t) {
        return cSeq((1 - c) * b + t * c);
    }

    double w(double x) {
        if (x >= -1  && x < 0)
            return 1 + x;
        if (x >= 0 && x <= 1)
            return 1 - x;
        return 0;
    }

    double S(int i) {
        double sti = 0;
        for (int j = 0; j < i; ++j) {
            sti += distance(cSeq(cast(double)j / K), cSeq(cast(double)(j + 1) / K));
        }
        return sti;
    }

    double T(double t) {
        double res = 0;
        for (int i = 0; i <= N; ++i) {
            const double si = S(i) / S(K);
            res += w(K*t-i)*si;
        }
        return res;
    }

    double temp = 0;
    for (int i = 0; i <= K; ++i) {
        //const double ti = cast(double)i / K;
        //const double si = S(i) / S(K);
        //const double t = T(ti);

        //writeln("ti ", ti);
        //writeln("si ", si);
        //writeln("t ", t);
        //generated[i] = cSeq(t);

        generated[i] = pSeq(temp);
        temp += 1.0 / K;
    }

    return generated;
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

void main(string[] args)
{
    int numColors = 8;
    double hue = 0;
    double s = 0.6;
    double b = 0.75;
    double c = 0.88 < 0.34 + 0.06*numColors ? 0.88 : 0.06*numColors;

    Main.init(args);
    Builder builder = new Builder();
    builder.addFromFile("design.glade");
    Window w = cast(Window)builder.getObject("main-window");
    w.addOnHide( delegate void(Widget){ Main.quit(); } );
    w.showAll();

    Box boxMain = cast(Box)builder.getObject("box-main");
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

    scaleSaturation.setValue(s);
    scaleContrast.setValue(c);
    scaleBrightness.setValue(b);

    SpinButton spinNumColors = cast(SpinButton)builder.getObject("spin-num-colors");
    spinNumColors.setValue(numColors);
    spinNumColors.addOnValueChanged(delegate void(SpinButton button) {
        numColors = to!int(button.getValue());
        s = 0.6;
        scaleSaturation.setValue(s);
        b = 0.75;
        scaleBrightness.setValue(b);
        c = 0.88 < 0.34 + 0.06*numColors ? 0.88 : 0.06*numColors;
        scaleContrast.setValue(c);
        onClickedGenerate(boxPalette, numColors, hue, s, b, c);
    });

    Main.run();
}
