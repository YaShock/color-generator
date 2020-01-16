#include "palette_widget.h"

#include "palette.h"

PaletteWidget::PaletteWidget(
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
	int* hue) :
	wxPanel(parent),
	numColors(numColors),
	gamma(gamma),
	type(type),
	M(M),
	M_INV(M_INV),
	contrast(contrast),
	saturation(saturation),
	brightness(brightness),
	coldWarm(coldWarm),
	hue(hue)
{
	sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	Layout();
}

void PaletteWidget::GeneratePalette()
{
	int N = *numColors;

	auto p = generateSeqPalette(
		*M,
		*gamma,
		*hue,
		*saturation,
		*brightness,
		*contrast,
		*coldWarm);
	auto colors = generateColors(p, N);

	Freeze();
	sizer->Clear();
	DestroyChildren();

	for (int i = 0; i < N; ++i)
	{
		auto colorLabel = new wxPanel(this);
		RGB color = xyzToRGB(luvToXYZ(lchToLUV(colors[i])), *M_INV, *gamma);
		colorLabel->SetBackgroundColour(wxColour(color.r * 255, color.g * 255, color.b * 255));
		colorLabel->SetMinSize(wxSize(100, 10));
		sizer->Add(colorLabel, 1, wxEXPAND|wxALL, 5);
	}

	Thaw();
}
