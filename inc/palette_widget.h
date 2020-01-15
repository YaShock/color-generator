#ifndef PALETTE_WIDGET_H
#define PALETTE_WIDGET_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

enum class PaletteType
{
	Sequential,
	Bivariate,
	Qualitative
};

class PaletteWidget : public wxPanel
{
public:
	PaletteWidget(
		wxWindow* parent,
		int* numColors,
		double* gamma,
		PaletteType* type,
		const double (*M)[3][3],
		const double (*M_INV)[3][3]);
	void GeneratePalette();

private:
	wxBoxSizer* sizer;

	int* numColors;
	double* gamma;
	PaletteType* type;
	const double (*M)[3][3];
	const double (*M_INV)[3][3];
};

#endif // PALETTE_WIDGET_H
