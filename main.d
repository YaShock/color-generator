pragma(lib, "gtkd-3");

import gtk.Builder;
import gtk.Main;
import gtk.Window;
import gtk.Scale;
import gtk.Range;
import gtk.Label;
import gdk.RGBA;
import std.stdio;
import std.math;

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
};

struct XYZ {
    double X, Y, Z;
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

LCH luvToLCH(LUV luv) {
    double H;
    if (atan2(luv.V, luv.U) >= 0)
        H = atan2(luv.V, luv.U)*180.0/PI;
    else
        H = atan2(luv.V, luv.U)*180.0/PI + 360;
    return LCH(luv.L, sqrt(luv.U*luv.U + luv.V*luv.V), H);
}

RGB MSC(double hue)
{
    double[][] M = [
        [0.5767309, 0.2973769, 0.0270343],
        [0.1855540, 0.6273491, 0.0706872],
        [0.1881852, 0.0752741, 0.9911085]
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

    const double u = 4*Xn/(Xn+15*Yn+3*Zn);
    const double v = 9*Yn/(Xn+15*Yn+3*Zn);
    const double gamma = 2.2;
    const double alpha = -sin(hue * PI / 180);
    const double beta = cos(hue * PI / 180);
    const double a = (alpha*u+beta*v)*(M[theta][0] + 15*M[theta][1] + 3*M[theta][2])-(4*alpha*M[theta][0]+9*beta*M[theta][1]);
    const double b = (alpha*u+beta*v)*(M[ro][0] + 15*M[ro][1] + 3*M[ro][2])-(4*alpha*M[ro][0]+9*beta*M[ro][1]);
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

void generatePalette(int numColors, double hue)
{
    const int N = numColors;
    const LCH p0 = LCH(0.0, 0.0, hue);
    const LCH p1 = luvToLCH(xyzToLUV(rgbToXYZ(MSC(hue), 2.2)));
    const LCH p2 = LCH(100.0, 0.0, hue);
    const double s = 0.6;
    const double b = 0.75;
    const double c = 0.88 < 0.34 + 0.06*N ? 0.88 : 0.06*N;
    const LCH q0 = (1 - s)*p0 + s*p1;
    const LCH q2 = (1 - s)*p2 + s*p1;
    const LCH q1 = 1.0/2.0*(q0 + q2);
    auto B0 = BezierLCH(p0, q0, q1);
    auto B1 = BezierLCH(q1, q2, p2);

    for (double t = 0.0; t < 1.0; t += 0.1) {
        LCH val;
        if (t <= 0.5)
            val = B0(2*t);
        else
            val = B1(t-0.5);
        writeln(val.L, ' ', val.C, ' ', val.H);
    }
}

void updateLabelMSC(Label labelMSC, Scale scale)
{
    const auto msc = MSC(scale.getValue());
    labelMSC.overrideBackgroundColor(GtkStateFlags.NORMAL, new RGBA(msc.r, msc.g, msc.b));
}

void main(string[] args)
{
    RGB color = MSC(280);
    generatePalette(5, 280);
    writeln("R ", color.r, " G ", color.g, " B ", color.b);
    Main.init(args);
    Builder builder = new Builder();
    builder.addFromFile("design.glade");
    Window w = cast(Window)builder.getObject("main-window");
    w.addOnHide( delegate void(Widget){ Main.quit(); } );
    w.showAll();

    Scale scaleHue = cast(Scale)builder.getObject("scale-hue");
    Label labelMSC = cast(Label)builder.getObject("label-msc");
    scaleHue.addOnValueChanged(delegate void(Range range) {
        updateLabelMSC(labelMSC, cast(Scale)(range));
    });

    Main.run();
}
