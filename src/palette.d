import utils;

import std.algorithm;
import std.math;

struct BezierLCH {
    LCH b0, b1, b2;

    this(LCH b0, LCH b1, LCH b2) {
        this.b0 = b0;
        this.b1 = b1;
        this.b2 = b2;
    }

    LCH opCall(double t) const {
        return (t - 1)*(t - 1)*b0 + 2*(1 - t)*t*b1 + t*t*b2;
    }
}

double distance(LCH c1, LCH c2) {
    return abs(log((125 - c2.L) / (125 - c1.L)));
}

interface Palette {
    LCH opCall(double t) const;
}

class SeqPalette : Palette {
    this(BezierLCH B0, BezierLCH B1, double b, double c)
    {
        this.B0 = B0;
        this.B1 = B1;
        this.b = b;
        this.c = c;
        calcAllS();
    }

    LCH opCall(double t) const {
        return pSeq(T(t));
    }

private:
    BezierLCH B0, B1;
    double b, c;
    const int K = 150;
    double[K + 1] sCache;

    LCH cSeq(double t) const {
        if (t <= 0.5)
            return B0(2*t);
        else
            return B1(2*(t-0.5));
    }

    LCH pSeq(double t) const {
        return cSeq((1 - c) * b + t * c);
    }

    double w(double x) const {
        if (x >= -1  && x < 0)
            return 1 + x;
        if (x >= 0 && x <= 1)
            return 1 - x;
        return 0;
    }

    double S(int i) const {
        double sti = 0;
        for (int j = 0; j < i; ++j) {
            sti += distance(cSeq(cast(double)j / K), cSeq(cast(double)(j + 1) / K));
        }
        return sti;
    }

    double T(double st) const {
        for (int i = 0; i < K; ++i) {
            const double sti = sCache[i] / sCache[K];
            const double stj = sCache[i + 1] / sCache[K];
            if (st >= sti && st <= stj) {
                const double ti = cast(double)i / K;
                const double tj = cast(double)(i + 1) / K;
                const double x = (st - sti) / (stj - sti);
                return x * (tj - ti) + ti;
            }
        }
        return 0;
    }

    void calcAllS() {
        for (int i = 0; i <= K; ++i) {
            sCache[i] = S(i);
        }
    }
}

class DivPalette : Palette
{
    this(const SeqPalette pSeq0, const SeqPalette pSeq1, LCH neutral, double m)
    {
        this.pSeq0 = pSeq0;
        this.pSeq1 = pSeq1;
        this.neutral = neutral;
        this.m = m;
    }

    override LCH opCall(double t) const {
        double t2;
        const double eps = m - 0.5;
        if (m >= 0.5) {
            t2 = t / (1 + abs(2.0*eps));
        }
        else {
            t2 = (t + abs(2.0*eps)) / (1 + abs(2.0*eps));
        }
        if (t2 < 0.5) {
            return pSeq0(2.0*t2);
        }
        if (t2 > 0.5) {
            return pSeq1(2.0*(1.0- t2));
        }
        return neutral;
    }

private:
    const SeqPalette pSeq0;
    const SeqPalette pSeq1;
    LCH neutral;
    double m;
}

LCH pb(double[3][3] M, double gamma) {
    return RGB(1, 1, 0).rgbToXYZ(M, gamma).xyzToLUV.luvToLCH;
}

double Smax(double[3][3] M, double gamma, double L, double H) {
    const LCH pMid = MSC(M, gamma, H).rgbToXYZ(M, gamma).xyzToLUV.luvToLCH;
    LCH pEnd;
    if (L <= pMid.L) {
        pEnd = LCH(0, 0, 0);
    }
    else {
        pEnd = LCH(100, 0, 0);
    }
    const double alpha = (pEnd.L - L) / (pEnd.L - pMid.L);
    return alpha * (pMid.C - pEnd.C) + pEnd.C;
}

SeqPalette generateSeqPalette(
    double[3][3] M,
    double gamma,
    double hue,
    double s,
    double b,
    double c,
    double w)
{
    double mixHue(double alpha, double h0, double h1) {
        const double M = (180 + h1 - h0) % 360 - 180;
        return (h0 + alpha*M) % 360;
    }
    const LCH p0 = LCH(0.0, 0.0, hue);
    const RGB msc = MSC(M, gamma, hue);
    const LCH p1 = luvToLCH(xyzToLUV(rgbToXYZ(msc, M, gamma)));
    LCH p2;
    const LCH pb = pb(M, gamma);
    p2.L = (1 - w)*100 + w*pb.L;
    p2.C = min(Smax(M, gamma, p2.L, p2.H), w * s * pb.C);
    p2.H = mixHue(w, hue, pb.H);
    const LCH q0 = (1 - s)*p0 + s*p1;
    const LCH q2 = (1 - s)*p2 + s*p1;
    const LCH q1 = 1.0/2.0*(q0 + q2);
    auto B0 = BezierLCH(p0, q0, q1);
    auto B1 = BezierLCH(q1, q2, p2);

    return new SeqPalette(B0, B1, b, c);
}

DivPalette generateDivPalette(
    double[3][3] M,
    double gamma,
    double h0,
    double h1,
    double s,
    double b,
    double c,
    double w,
    double m)
{
    const SeqPalette pSeq0 = generateSeqPalette(M, gamma, h0, s, b, c, w);
    const SeqPalette pSeq1 = generateSeqPalette(M, gamma, h1, s, b, c, w);
    const LCH c0 = pSeq0(1);
    const LCH c1 = pSeq1(1);
    const LCH pb = pb(M, gamma);
    LCH cn;
    const double sn = (c0.C + c1.C) * w / 2.0;
    cn.L = (c0.L + c1.L) / 2.0;
    cn.H = pb.H;
    cn.C = min(Smax(M, gamma, cn.L, cn.H), sn);
    return new DivPalette(pSeq0, pSeq1, cn, m);
}

LCH[] generateColors(Palette palette, int numColors)
{
    const int N = numColors;
    LCH[] generated = new LCH[N];

    for (int i = 0; i < N; ++i) {
        const double ti = cast(double)i / (N - 1);
        generated[i] = palette(ti);
    }

    return generated;
}
