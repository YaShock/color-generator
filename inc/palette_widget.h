#ifndef PALETTE_WIDGET_H
#define PALETTE_WIDGET_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <memory>

enum class PaletteType
{
	Sequential,
	Bivariate,
	Qualitative
};

class Palette;

class PaletteWidget : public wxPanel
{
	class DrawPanel;
public:
	PaletteWidget(
		wxWindow* parent,
		int* numColors,
		double* gamma,
		PaletteType* type,
		const double (*M)[3][3],
		const double (*M_INV)[3][3],
		double* contrast,
		double* saturation,
		double* brightness,
		double* coldWarm,
		int* hue);
	void GeneratePalette();

private:
	wxBoxSizer* sizer;
	DrawPanel* drawPanel;

	std::unique_ptr<Palette> palette;

	int* numColors;
	double* gamma;
	PaletteType* type;
	const double (*M)[3][3];
	const double (*M_INV)[3][3];

	double* contrast;
	double* saturation;
	double* brightness;
	double* coldWarm;
	int* hue;
};

#endif // PALETTE_WIDGET_H
