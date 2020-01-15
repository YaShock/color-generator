#include "palette_widget.h"

#include "palette.h"

PaletteWidget::PaletteWidget(
	wxWindow* parent,
	int* numColors,
	double* gamma,
	PaletteType* type,
	const double (*M)[3][3],
	const double (*M_INV)[3][3]) :
	wxPanel(parent),
	numColors(numColors),
	gamma(gamma),
	type(type),
	M(M),
	M_INV(M_INV)
{
	sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	Layout();
}

void PaletteWidget::GeneratePalette()
{
	int N = *numColors;

	double c = 0.88 < 0.34 + 0.06 * N ? 0.88 : 0.06 * N;
	auto p = generateSeqPalette(
		*M,
		2.2,
		150,
		0.6,
		0.75,
		c,
		0.15);
	auto colors = generateColors(p, N);

	sizer->Clear();

	for (int i = 0; i < N; ++i)
	{
		auto colorLabel = new wxPanel(this);
		RGB color = xyzToRGB(luvToXYZ(lchToLUV(colors[i])), *M_INV, 2.2);
		colorLabel->SetBackgroundColour(wxColour(color.r * 255, color.g * 255, color.b * 255));
		colorLabel->SetMinSize(wxSize(100, 10));
		sizer->Add(colorLabel, 1, wxEXPAND|wxALL, 5);
	}
}
