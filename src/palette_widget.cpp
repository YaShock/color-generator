#include "palette_widget.h"

#include "palette.h"
#include <wx/dcbuffer.h>

class PaletteWidget::DrawPanel : public wxPanel
{
public:
	DrawPanel(
		std::unique_ptr<Palette>& palette,
		std::function<void(const RGB&)>& cb,
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize);
	void OnPaint(wxPaintEvent&);
	void OnErase(wxEraseEvent&);
	void OnClick(wxMouseEvent& event);
	void Render(wxDC& dc);

	DECLARE_EVENT_TABLE()

private:
	std::unique_ptr<Palette>& palette;
	std::function<void(const RGB&)>& colorClicked;
};

BEGIN_EVENT_TABLE(PaletteWidget::DrawPanel, wxPanel)
	EVT_PAINT(PaletteWidget::DrawPanel::OnPaint)
	EVT_ERASE_BACKGROUND(PaletteWidget::DrawPanel::OnErase)
	EVT_LEFT_DOWN(PaletteWidget::DrawPanel::OnClick)
END_EVENT_TABLE()

PaletteWidget::DrawPanel::DrawPanel(
	std::unique_ptr<Palette>& palette,
	std::function<void(const RGB&)>& cb,
	wxWindow* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size) :
	wxPanel(parent, id, pos, size),
	palette(palette),
	colorClicked(cb)
{
}

void PaletteWidget::DrawPanel::OnErase(wxEraseEvent&)
{
}

void PaletteWidget::DrawPanel::OnPaint(wxPaintEvent&)
{
	wxAutoBufferedPaintDC dc(this);
	Render(dc);
}

void PaletteWidget::DrawPanel::OnClick(wxMouseEvent& event)
{
	auto clientSize = GetClientSize();
	auto x = event.GetX();
	double t = double(x) / clientSize.GetWidth();

	RGB rgb = xyzToRGB(luvToXYZ(lchToLUV((*palette)(t))), M_INV_SRGB, 2.2);
	if (colorClicked)
		colorClicked(rgb);
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
	const double(**M_INV)[3][3],
	std::unique_ptr<Palette>& palette) :
	wxPanel(parent),
	numColors(numColors),
	gamma(gamma),
	M_INV(M_INV),
	palette(palette)
{
	sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	Layout();
	GeneratePalette();
}

void PaletteWidget::GeneratePalette()
{
	int N = *numColors;
	auto colors = generateColors(*palette, N);

	Freeze();
	sizer->Clear();
	DestroyChildren();

	drawPanel = new DrawPanel(palette, colorClicked, this);
	sizer->Add(drawPanel, 2, wxEXPAND | wxALL, 5);

	for (int i = 0; i < N; ++i)
	{
		auto colorLabel = new wxPanel(this);
		RGB color = xyzToRGB(luvToXYZ(lchToLUV(colors[i])), **M_INV, *gamma);
		colorLabel->SetBackgroundColour(wxColour(color.r * 255, color.g * 255, color.b * 255));
		colorLabel->SetMinSize(wxSize(100, 10));
		sizer->Add(colorLabel, 1, wxEXPAND|wxALL, 5);

		colorLabel->Bind(wxEVT_LEFT_DOWN,
			[this, color](wxMouseEvent&){
				if (colorClicked) {
					colorClicked(color);
				}
			});
	}

	Thaw();
}

void PaletteWidget::SetColorCallback(std::function<void(const RGB&)> cb)
{
	colorClicked = cb;
}
