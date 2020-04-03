#include "palette.h"

#include <cmath>
#include <algorithm>

LCH BezierLCH::operator()(double t) const {
	return b0*((t - 1)*(t - 1)) + b1*(2*(1 - t)*t) + b2*(t*t);
}

double distance(LCH c1, LCH c2) {
	return std::abs(std::log((125 - c2.L) / (125 - c1.L)));
}

SeqPalette::SeqPalette(BezierLCH B0, BezierLCH B1, double b, double c)
	: B0(B0), B1(B1), b(b), c(c)
{
	calcAllS();
}

LCH SeqPalette::operator()(double t) const
{
	return pSeq(T(t));
}

LCH SeqPalette::cSeq(double t) const
{
	if (t <= 0.5)
		return B0(2*t);
	else
		return B1(2*(t-0.5));
}

LCH SeqPalette::pSeq(double t) const
{
	return cSeq((1 - c) * b + t * c);
}

double SeqPalette::w(double x) const
{
	if (x >= -1  && x < 0)
		return 1 + x;
	if (x >= 0 && x <= 1)
		return 1 - x;
	return 0;
}

double SeqPalette::S(int i) const
{
	double sti = 0;
	for (int j = 0; j < i; ++j) {
		sti += distance(cSeq(double(j) / K), cSeq(double(j + 1) / K));
	}
	return sti;
}

double SeqPalette::T(double st) const
{
	for (int i = 0; i < K; ++i) {
		const double sti = sCache[i] / sCache[K];
		const double stj = sCache[i + 1] / sCache[K];
		if (st >= sti && st <= stj) {
			const double ti = double(i) / K;
			const double tj = double(i + 1) / K;
			const double x = (st - sti) / (stj - sti);
			return x * (tj - ti) + ti;
		}
	}
	return 0;
}

void SeqPalette::calcAllS()
{
	for (int i = 0; i <= K; ++i) {
		sCache[i] = S(i);
	}
}

DivPalette::DivPalette(SeqPalette pSeq0, SeqPalette pSeq1, LCH neutral, double m)
	: pSeq0(pSeq0), pSeq1(pSeq1), neutral(neutral), m(m)
{
}

LCH DivPalette::operator()(double t) const
{
	double t2;
	const double eps = m - 0.5;
	if (m >= 0.5) {
		t2 = t / (1 + std::abs(2.0*eps));
	}
	else {
		t2 = (t + std::abs(2.0*eps)) / (1.0 + std::abs(2.0*eps));
	}
	if (t2 < 0.5) {
		return pSeq0(2.0*t2);
	}
	if (t2 > 0.5) {
		return pSeq1(2.0*(1.0- t2));
	}
	return neutral;
}

QualPalette::QualPalette(
	const double M[][3],
	double gamma,
	double eps,
	double r,
	double s,
	double b,
	double c)
	: gamma(gamma), eps(eps), r(r), s(s), b(b), c(c)
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			this->M[i][j] = M[i][j];
	cy = luvToLCH(xyzToLUV(rgbToXYZ(RGB{1, 1, 0}, M, gamma)));
	maxs = luvToLCH(xyzToLUV(rgbToXYZ(RGB{1, 0, 0}, M, gamma))).C;
	l0 = b*cy.L;
	l1 = (1 - c) * l0;
}

LCH QualPalette::operator()(double t) const {
	double h = int(360 * (eps + t*r)) % 360;
	double alpha = hueDiff(h, cy.H) / 360;
	double l = (1 - alpha) * l0 + alpha * l1;
	double c = std::min(Smax(M, gamma, l, h), s * maxs);
	return LCH{l, c, h};
}

LCH pb(const double M[][3], double gamma)
{
	return luvToLCH(xyzToLUV(rgbToXYZ(RGB{1, 1, 0}, M, gamma)));
}

double Smax(const double M[][3], double gamma, double L, double H)
{
	const LCH pMid = luvToLCH(xyzToLUV(rgbToXYZ(MSC(M, gamma, H), M, gamma)));
	LCH pEnd;
	if (L <= pMid.L) {
		pEnd = LCH{0, 0, 0};
	}
	else {
		pEnd = LCH{100, 0, 0};
	}
	const double alpha = (pEnd.L - L) / (pEnd.L - pMid.L);
	return alpha * (pMid.C - pEnd.C) + pEnd.C;
}

double mixHue(double alpha, double h0, double h1)
{
	const double M = int(180 + h1 - h0) % 360 - 180;
	return int(h0 + alpha*M) % 360;
}

SeqPalette generateSeqPalette(
	const double M[][3],
	double gamma,
	double hue,
	double s,
	double b,
	double c,
	double w)
{
	const LCH p0 = LCH{0.0, 0.0, hue};
	const RGB msc = MSC(M, gamma, hue);
	const LCH p1 = luvToLCH(xyzToLUV(rgbToXYZ(msc, M, gamma)));
	const LCH _pb = pb(M, gamma);
	double p2L = (1 - w) * 100 + w * _pb.L;
	double p2H = mixHue(w, hue, _pb.H);
	double p2C = std::min(Smax(M, gamma, p2L, p2H), w * s * _pb.C);
	LCH p2{ p2L, p2C, p2H };
	const LCH q0 = p0*(1 - s) + p1*s;
	const LCH q2 = p2*(1 - s) + p1*s;
	const LCH q1 = (q0 + q2)*(1.0/2.0);
	auto B0 = BezierLCH{p0, q0, q1};
	auto B1 = BezierLCH{q1, q2, p2};

	return SeqPalette(B0, B1, b, c);
}

DivPalette generateDivPalette(
	const double M[][3],
	double gamma,
	double h0,
	double h1,
	double s,
	double b,
	double c,
	double w,
	double m)
{
	SeqPalette pSeq0 = generateSeqPalette(M, gamma, h0, s, b, c, w);
	SeqPalette pSeq1 = generateSeqPalette(M, gamma, h1, s, b, c, w);
	const LCH c0 = pSeq0(1);
	const LCH c1 = pSeq1(1);
	const LCH _pb = pb(M, gamma);
	LCH cn;
	const double sn = (c0.C + c1.C) * w / 2.0;
	cn.L = (c0.L + c1.L) / 2.0;
	cn.H = _pb.H;
	cn.C = std::min(Smax(M, gamma, cn.L, cn.H), sn);
	return DivPalette{pSeq0, pSeq1, cn, m};
}

QualPalette generateQualPalette(
	const double M[][3],
	double gamma,
	double eps,
	double r,
	double s,
	double b,
	double c)
{
	return QualPalette(M, gamma, eps, r, s, b, c);
}

std::vector<LCH> generateColors(Palette& palette, int numColors)
{
	const int N = numColors;
	std::vector<LCH> generated(N);

	for (int i = 0; i < N; ++i) {
		const double ti = double(i) / (N - 1);
		generated[i] = palette(ti);
	}

	return generated;
}