#ifndef PALETTE_WIDGET_H
#define PALETTE_WIDGET_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <memory>
#include <functional>
#include "utils.h"

class Palette;

class PaletteWidget : public wxPanel
{
	class DrawPanel;
public:
	PaletteWidget(
		wxWindow* parent,
		int* numColors,
		double* gamma,
		const double(**M_INV)[3][3],
		std::unique_ptr<Palette>& palette);
	void GeneratePalette();
	void SetColorCallback(std::function<void(const RGB&)> cb);

	wxBoxSizer* sizer;
	DrawPanel* drawPanel;
	std::unique_ptr<Palette>& palette;

	int* numColors;
	double* gamma;
	const double(**M_INV)[3][3];

	std::function<void(const RGB&)> colorClicked;
};

#endif // PALETTE_WIDGET_H
