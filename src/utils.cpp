#include "utils.h"

// Needed for MS compiler
#define _USE_MATH_DEFINES

#include <cmath>
#include <stdexcept>
#include <algorithm>


const double M_SRGB[3][3] = {
    {0.4124564, 0.3575761, 0.1804375},
    {0.2126729, 0.7151522, 0.0721750},
    {0.0193339, 0.1191920, 0.9503041}
};

const double M_INV_SRGB[3][3] = {
    {3.2404542,-1.5371385, -0.4985314},
    {-0.9692660, 1.8760108, 0.0415560},
    {0.0556434, -0.2040259, 1.0572252}
};

const double M_ADOBE[3][3] = {
    {0.5767309, 0.1855540, 0.1881852},
    {0.2973769, 0.6273491, 0.0752741},
    {0.0270343, 0.0706872, 0.9911085}
};

const double M_INV_ADOBE[3][3] = {
    {2.0413690, -0.5649464, -0.3446944},
    {-0.9692660, 1.8760108, 0.0415560},
    {0.0134474, -0.1183897, 1.0154096}
};

double& RGB::operator[](int i) {
    if (i == 0)
        return r;
    if (i == 1)
        return g;
    if (i == 2)
        return b;
    throw std::out_of_range("Invalid index");
}

const double& RGB::operator[](int i) const {
    if (i == 0)
        return r;
    if (i == 1)
        return g;
    if (i == 2)
        return b;
    throw std::out_of_range("Invalid index");
}

RGB RGB::operator*(double s) const {
    return RGB{s*r, s*g, s*b};
}

bool RGB::isValid() const
{
    return r >= 0 && r <= 1 &&
           g >= 0 && g <= 1 &&
           b >= 0 && b <= 1;
}

double& XYZ::operator[](int i) {
    if (i == 0)
        return X;
    if (i == 1)
        return Y;
    if (i == 2)
        return Z;
    throw std::out_of_range("Invalid index");
}

const double& XYZ::operator[](int i) const {
    if (i == 0)
        return X;
    if (i == 1)
        return Y;
    if (i == 2)
        return Z;
    throw std::out_of_range("Invalid index");
}

LCH LCH::operator*(double s) const {
    return LCH{s*L, s*C, s*H};
}

LCH LCH::operator+(const LCH& rhs) const {
    return LCH{L + rhs.L, C + rhs.C, H + rhs.H};
}

HSV rgbToHSV(const RGB& rgb)
{
    const double Cmin = std::min(std::min(rgb.r, rgb.g), rgb.b);
    const double Cmax = std::max(std::max(rgb.r, rgb.g), rgb.b);
    const double delta = Cmax - Cmin;
    HSV hsv;
    if (Cmax == rgb.r) {
        hsv.H = 60.0*(fmod((rgb.g - rgb.b) / delta, 6));
    }
    else if (Cmax == rgb.g) {
        hsv.H = 60.0*(2 + (rgb.b - rgb.r) / delta);
    }
    else if (Cmax == rgb.b) {
        hsv.H = 60.0*(4 + (rgb.r - rgb.g) / delta);
    }
    else {
        hsv.H = 0;
    }
    if (Cmax == 0) {
        hsv.S = 0;
    }
    else {
        hsv.S = delta / Cmax;
    }
    hsv.V = Cmax;
    return hsv;
}

RGB hsvToRGB(const HSV& hsv)
{
    const double C = hsv.V * hsv.S;
    const double X = C * (1 - std::abs(fmod(hsv.H / 60.0, 2.0) - 1));
    const double m = hsv.V - C;
    double r, g, b;
    if (hsv.H >= 0 && hsv.H < 60) {
        r = C;
        g = X;
        b = 0;
    }
    else if (hsv.H >= 60 && hsv.H < 120) {
        r = X;
        g = C;
        b = 0;
    }
    else if (hsv.H >= 120 && hsv.H < 180) {
        r = 0;
        g = C;
        b = X;
    }
    else if (hsv.H >= 180 && hsv.H < 240) {
        r = 0;
        g = X;
        b = C;
    }
    else if (hsv.H >= 240 && hsv.H < 300) {
        r = X;
        g = 0;
        b = C;
    }
    else {
        r = C;
        g = 0;
        b = X;
    }
    return RGB{r + m, g + m, b + m};
}

XYZ rgbToXYZ(const RGB& rgb, const double M[][3], double gamma) {
    double xyz[] = {0, 0, 0};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            xyz[i] += std::pow(rgb[j], gamma) * M[i][j];
        }
    }
    return XYZ{xyz[0], xyz[1], xyz[2]};
}

RGB xyzToRGB(const XYZ& xyz, const double Minv[][3], double gamma) {
    double rgb[] = {0, 0, 0};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rgb[i] += xyz[j] * Minv[i][j];
        }
        rgb[i] = std::min(std::max(rgb[i], 0.0), 1.0);
        rgb[i] = std::pow(rgb[i], 1.0 / gamma);
    }
    return RGB{rgb[0], rgb[1], rgb[2]};
}

LUV xyzToLUV(const XYZ& xyz) {
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
        L = 116*std::pow(yn, 1.0/3.0)-16;
    else
        L = ki*yn;
    return LUV{L, 13*L*(U-Un), 13*L*(V-Vn)};
}

XYZ luvToXYZ(const LUV& luv) {
    const double Xn = 0.95047;
    const double Yn = 1.0;
    const double Zn = 1.08883;
    const double eps = 0.008856;
    const double ki = 903.3;
    const double L = luv.L;

    const double u0 = 4*Xn/(Xn + 15*Yn + 3*Zn);
    const double v0 = 9*Yn/(Xn + 15*Yn + 3*Zn);
    const double Y = L > ki*eps ? std::pow(((L + 16) / 116.0), 3.0) : L / ki;

    const double a = L > 0 ? 1.0/3.0*(52*L/(luv.U + 13*L*u0)-1) : 0;
    const double b = -5*Y;
    const double c = -1.0/3.0;
    const double d = luv.V + 13*L*v0 > 0 ? Y*(39*L/(luv.V + 13*L*v0)-5) : 0;

    const double X = (d - b) / (a - c);
    const double Z = X*a + b;


    return XYZ{X, Y, Z};
}

LCH luvToLCH(const LUV& luv) {
    double H;
    if (atan2(luv.V, luv.U) >= 0)
        H = std::atan2(luv.V, luv.U)*180.0/M_PI;
    else
        H = std::atan2(luv.V, luv.U)*180.0/M_PI + 360;
    return LCH{luv.L, sqrt(luv.U*luv.U + luv.V*luv.V), H};
}

LUV lchToLUV(const LCH& lch) {
    const double C = lch.C;
    const double H = lch.H;
    return LUV{lch.L, C * cos(H * M_PI / 180), C * sin(H * M_PI / 180)};
}

RGB MSC(const double M[][3], double gamma, double hue)
{
    const double Xn = 0.95047;
    const double Yn = 1.0;
    const double Zn = 1.08883;
    int ro, sigma, theta;
    const double hRed = luvToLCH(xyzToLUV(rgbToXYZ(RGB{1.0, 0.0, 0.0}, M, gamma))).H;
    const double hYellow = luvToLCH(xyzToLUV(rgbToXYZ(RGB{1.0, 1.0, 0.0}, M, gamma))).H;
    const double hGreen = luvToLCH(xyzToLUV(rgbToXYZ(RGB{0.0, 1.0, 0.0}, M, gamma))).H;
    const double hCyan = luvToLCH(xyzToLUV(rgbToXYZ(RGB{0.0, 1.0, 1.0}, M, gamma))).H;
    const double hBlue = luvToLCH(xyzToLUV(rgbToXYZ(RGB{0.0, 0.0, 1.0}, M, gamma))).H;
    const double hMagenta = luvToLCH(xyzToLUV(rgbToXYZ(RGB{1.0, 0.0, 1.0}, M, gamma))).H;
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
    const double alpha = -std::sin(hue * M_PI / 180);
    const double beta = std::cos(hue * M_PI / 180);
    const double a = (alpha*Un+beta*Vn)*(M[0][theta] + 15*M[1][theta] + 3*M[2][theta])-(4*alpha*M[0][theta]+9*beta*M[1][theta]);
    const double b = (alpha*Un+beta*Vn)*(M[0][ro] + 15*M[1][ro] + 3*M[2][ro])-(4*alpha*M[0][ro]+9*beta*M[1][ro]);
    const double roValue = std::pow(-a/b, 1.0/gamma);

    auto color = RGB{0, 0, 0};
    color[sigma] = 0;
    color[theta] = 1;
    color[ro] = roValue;
    return color;
}

double hueDiff(double h0, double h1)
{
    h0 = int(h0) % 360;
    h1 = int(h1) % 360;
    if (std::abs(h1 - h0) < 180.0)
        return std::abs(h1 - h0);
    else
        return 360 - std::abs(h1 - h0);
}
