#ifndef UTILS_H
#define UTILS_H

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

struct RGB {
    double r, g, b;

    double& operator[](int i);
    const double& operator[](int i) const;
    RGB operator*(double s) const;
    bool isValid() const;
};

struct XYZ {
    double X, Y, Z;

    double& operator[](int i);
    const double& operator[](int i) const;
};

struct LUV {
    double L, U, V;
};

struct LCH {
    double L, C, H;

    LCH operator*(double s) const;
    LCH operator+(const LCH& rhs) const;
};

struct HSV {
    double H, S, V;
};

HSV rgbToHSV(const RGB& rgb);
RGB hsvToRGB(const HSV& hsv);
XYZ rgbToXYZ(const RGB& rgb, double M[][3], double gamma);
RGB xyzToRGB(const XYZ& xyz, double Minv[][3], double gamma);
LUV xyzToLUV(const XYZ& xyz);
XYZ luvToXYZ(const LUV& luv);
LCH luvToLCH(const LUV& luv);
LUV lchToLUV(const LCH& lch);

RGB MSC(double M[][3], double gamma, double hue);
double hueDiff(double h0, double h1);

#endif // UTILS_H
