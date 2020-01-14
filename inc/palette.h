#ifndef PALETTE_H
#define PALETTE_H

#include "utils.h"

#include <vector>

struct BezierLCH
{
	LCH b0, b1, b2;

	LCH operator()(double t) const;
};

double distance(LCH c1, LCH c2);

class Palette
{
public:
	virtual LCH operator()(double t) const = 0;
};

class SeqPalette : public Palette
{
	BezierLCH B0, B1;
	double b, c;
	static const int K = 150;
	double sCache[K + 1];

public:
	SeqPalette(BezierLCH B0, BezierLCH B1, double b, double c);
	LCH operator()(double t) const override;

private:
	LCH cSeq(double t) const;
	LCH pSeq(double t) const;
	double w(double x) const;
	double S(int i) const;
	double T(double st) const;

	void calcAllS();
};

class DivPalette : public Palette
{
	SeqPalette pSeq0;
	SeqPalette pSeq1;
	LCH neutral;
	double m;

public:
	DivPalette(SeqPalette pSeq0, SeqPalette pSeq1, LCH neutral, double m);
	LCH operator()(double t) const override;
};

class QualPalette : public Palette
{
	LCH cy;
	double M[3][3];
	double gamma, eps, r, s, b, c, l0, l1, maxs;

public:
	QualPalette(
		const double M[][3],
		double gamma,
		double eps,
		double r,
		double s,
		double b,
		double c);
	LCH operator()(double t) const override;
};

LCH pb(const double M[][3], double gamma);
double Smax(const double M[][3], double gamma, double L, double H);
double mixHue(double alpha, double h0, double h1);

SeqPalette generateSeqPalette(
	const double M[][3],
	double gamma,
	double hue,
	double s,
	double b,
	double c,
	double w);
DivPalette generateDivPalette(
	const double M[][3],
	double gamma,
	double h0,
	double h1,
	double s,
	double b,
	double c,
	double w,
	double m);
QualPalette generateQualPalette(
	const double M[][3],
	double gamma,
	double eps,
	double r,
	double s,
	double b,
	double c);
std::vector<LCH> generateColors(Palette& palette, int numColors);

#endif // PALETTE_H
