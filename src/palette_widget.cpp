#include "palette_widget.h"

#include "palette.h"

class PaletteWidget::DrawPanel : public wxPanel
{
public:
	DrawPanel(
		std::unique_ptr<Palette>& palette,
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize);
	void OnPaint(wxPaintEvent&);
	void Render(wxDC& dc);

	DECLARE_EVENT_TABLE()

private:
	std::unique_ptr<Palette>& palette;
};

BEGIN_EVENT_TABLE(PaletteWidget::DrawPanel, wxPanel)
	EVT_PAINT(PaletteWidget::DrawPanel::OnPaint)
END_EVENT_TABLE()

PaletteWidget::DrawPanel::DrawPanel(
	std::unique_ptr<Palette>& palette,
	wxWindow* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size) :
	wxPanel(parent, id, pos, size),
	palette(palette)
{
}

void PaletteWidget::DrawPanel::OnPaint(wxPaintEvent&)
{
	wxClientDC dc(this);
	Render(dc);
}

void PaletteWidget::DrawPanel::Render(wxDC& dc)
{
	auto clientSize = GetClientSize();

	for (int x = 0; x < clientSize.GetWidth(); ++x) {
		double t = double(x) / clientSize.GetWidth();
		RGB rgb = xyzToRGB(luvToXYZ(lchToLUV((*palette)(t))), M_INV_SRGB, 2.2);
		dc.SetPen(wxPen(wxColour(rgb.r * 255, rgb.g * 255, rgb.b * 255), 1));
		dc.DrawLine(x, 0, x, clientSize.GetHeight());
	}
}

PaletteWidget::PaletteWidget(
	wxWindow* parent,
	int* numColors,
	double* gamma,
	PaletteType* type,
	const double (**M)[3][3],
	const double (**M_INV)[3][3],
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
	GeneratePalette();
}

void PaletteWidget::GeneratePalette()
{
	int N = *numColors;

	palette = std::make_unique<SeqPalette>(generateSeqPalette(
		**M,
		*gamma,
		*hue,
		*saturation,
		*brightness,
		*contrast,
		*coldWarm));
	auto colors = generateColors(*palette, N);

	Freeze();
	sizer->Clear();
	DestroyChildren();

	drawPanel = new DrawPanel(palette, this);
	sizer->Add(drawPanel, 2, wxEXPAND | wxALL, 5);

	for (int i = 0; i < N; ++i)
	{
		auto colorLabel = new wxPanel(this);
		RGB color = xyzToRGB(luvToXYZ(lchToLUV(colors[i])), **M_INV, *gamma);
		colorLabel->SetBackgroundColour(wxColour(color.r * 255, color.g * 255, color.b * 255));
		colorLabel->SetMinSize(wxSize(100, 10));
		sizer->Add(colorLabel, 1, wxEXPAND|wxALL, 5);
	}

	Thaw();
}
