import std.algorithm;
import std.math;
import std.stdio;

const double[3][3] M_SRGB = [
    [0.4124564, 0.3575761, 0.1804375],
    [0.2126729, 0.7151522, 0.0721750],
    [0.0193339, 0.1191920, 0.9503041]
];

const double[3][3] M_INV_SRGB = [
    [3.2404542,-1.5371385, -0.4985314],
    [-0.9692660, 1.8760108, 0.0415560],
    [0.0556434, -0.2040259, 1.0572252]
];

const double[3][3] M_ADOBE = [
    [0.5767309, 0.1855540, 0.1881852],
    [0.2973769, 0.6273491, 0.0752741],
    [0.0270343, 0.0706872, 0.9911085]
];

const double[3][3] M_INV_ADOBE = [
    [2.0413690, -0.5649464, -0.3446944],
    [-0.9692660, 1.8760108, 0.0415560],
    [0.0134474, -0.1183897, 1.0154096]
];

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

    bool isValid()
    {
        return r >= 0 && r <= 1 &&
               g >= 0 && g <= 1 &&
               b >= 0 && b <= 1;
    }
}

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
}

struct LUV {
    double L, U, V;
}

struct LCH {
    double L, C, H;

    LCH opMul(double s) const {
        return LCH(s*L, s*C, s*H);
    }

    LCH opAdd(LCH rhs) const {
        return LCH(L + rhs.L, C + rhs.C, H + rhs.H);
    }
}

XYZ rgbToXYZ(RGB rgb, double[3][3] M, double gamma) {
    double[] xyz = [0, 0, 0];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            xyz[i] += pow(rgb[j], gamma) * M[i][j];
        }
    }
    return XYZ(xyz[0], xyz[1], xyz[2]);
}

RGB xyzToRGB(XYZ xyz, double[3][3] Minv, double gamma) {
    double[] rgb = [0, 0, 0];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rgb[i] += xyz[j] * Minv[i][j];
        }
        rgb[i] = pow(rgb[i], 1.0 / gamma);
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
        L = 116*pow(yn, 1.0/3.0)-16;
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

RGB MSC(double[3][3] M, double gamma, double hue)
{
    const double Xn = 0.95047;
    const double Yn = 1.0;
    const double Zn = 1.08883;
    int ro, sigma, theta;
    const double hRed = RGB(1.0, 0.0, 0.0).rgbToXYZ(M, gamma).xyzToLUV.luvToLCH.H;
    const double hYellow = RGB(1.0, 1.0, 0.0).rgbToXYZ(M, gamma).xyzToLUV.luvToLCH.H;
    const double hGreen = RGB(0.0, 1.0, 0.0).rgbToXYZ(M, gamma).xyzToLUV.luvToLCH.H;
    const double hCyan = RGB(0.0, 1.0, 1.0).rgbToXYZ(M, gamma).xyzToLUV.luvToLCH.H;
    const double hBlue = RGB(0.0, 0.0, 1.0).rgbToXYZ(M, gamma).xyzToLUV.luvToLCH.H;
    const double hMagenta = RGB(1.0, 0.0, 1.0).rgbToXYZ(M, gamma).xyzToLUV.luvToLCH.H;
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

double hueDiff(double h0, double h1)
{
    h0 = h0 % 360;
    h1 = h1 % 360;
    if (abs(h1 - h0) < 180.0)
        return abs(h1 - h0);
    else
        return 360 - abs(h1 - h0);
}
