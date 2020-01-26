#include "color_picker.h"

namespace
{

void AddSpin(
	wxWindow* parent,
	wxBoxSizer* sizer,
	wxSpinCtrlDouble** spin,
	const wxString& label)
{
	wxBoxSizer* rowSizer = new wxBoxSizer(wxHORIZONTAL);

	auto staticText = new wxStaticText(parent, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
	staticText->Wrap(-1);
	rowSizer->Add(staticText, 0, wxALIGN_CENTER|wxALL, 5);

	*spin = new wxSpinCtrlDouble(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1);
	(*spin)->SetDigits(0);
	rowSizer->Add(*spin, 1, wxALIGN_CENTER, 5);

	sizer->Add(rowSizer, 1, wxEXPAND, 5);
}

void AddGroup(
	wxWindow* parent,
	wxBoxSizer* mainSizer,
	std::vector<std::pair<wxSpinCtrlDouble**, wxString>> controls)
{
	wxBoxSizer* groupSizer = new wxBoxSizer(wxVERTICAL);
	for (auto& control : controls) {
		AddSpin(parent, groupSizer, control.first, control.second);
	}
	mainSizer->Add(groupSizer, 1, wxEXPAND, 0);
}

}

ColorPicker::ColorPicker(wxWindow* parent)
	: wxPanel(parent)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	colorLabel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	colorLabel->SetMinSize(wxSize(100,100));
	colorLabel->SetBackgroundColour(wxColour(0, 0, 0));

	mainSizer->Add(colorLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	AddGroup(this, mainSizer, {
		{ &spinHsvHue, wxT("Hue") },
		{ &spinHsvSat, wxT("Saturation") },
		{ &spinHsvVal, wxT("Value") }
	});

	AddGroup(this, mainSizer, {
		{ &spinRgbRed, wxT("Red") },
		{ &spinRgbGreen, wxT("Green") },
		{ &spinRgbBlue, wxT("Blue") }
	});

	AddGroup(this, mainSizer, {
		{ &spinXyzX, wxT("X") },
		{ &spinXyzY, wxT("Y") },
		{ &spinXyzZ, wxT("Z") }
	});

	AddGroup(this, mainSizer, {
		{ &spinLuvL, wxT("L") },
		{ &spinLuvU, wxT("U") },
		{ &spinLuvV, wxT("V") }
	});

	AddGroup(this, mainSizer, {
		{ &spinLchL, wxT("L") },
		{ &spinLchC, wxT("C") },
		{ &spinLchH, wxT("H") }
	});

	this->SetSizer(mainSizer);
	this->Layout();
}

void ColorPicker::SetColor(const RGB& color)
{
	spinRgbRed->SetValue(int(color.r * 100));
	spinRgbGreen->SetValue(int(color.g * 100));
	spinRgbBlue->SetValue(int(color.b * 100));

	colorLabel->SetBackgroundColour(wxColour(color.r * 255, color.g * 255, color.b * 255));
}
