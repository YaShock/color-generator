#ifndef UTILS_H
#define UTILS_H

extern const double M_SRGB[3][3];
extern const double M_INV_SRGB[3][3];
extern const double M_ADOBE[3][3];
extern const double M_INV_ADOBE[3][3];

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
XYZ rgbToXYZ(const RGB& rgb, const double M[][3], double gamma);
RGB xyzToRGB(const XYZ& xyz, const double Minv[][3], double gamma);
LUV xyzToLUV(const XYZ& xyz);
XYZ luvToXYZ(const LUV& luv);
LCH luvToLCH(const LUV& luv);
LUV lchToLUV(const LCH& lch);

RGB MSC(const double M[][3], double gamma, double hue);
double hueDiff(double h0, double h1);

#endif // UTILS_H
