#include "color_picker.h"

void ColorPicker::AddSpin(
	wxBoxSizer* sizer,
	const SpinValues& values)
{
	wxBoxSizer* rowSizer = new wxBoxSizer(wxHORIZONTAL);

	auto staticText = new wxStaticText(this, wxID_ANY, values.name, wxDefaultPosition, wxDefaultSize, 0);
	staticText->Wrap(-1);
	rowSizer->Add(staticText, 0, wxALIGN_CENTER|wxALL, 5);

	*values.widget = new wxSpinCtrlDouble(
		this,
		values.id,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		values.min,
		values.max,
		0,
		values.inc);
	(*values.widget)->SetDigits(values.digits);
	(*values.widget)->Bind(wxEVT_SPINCTRLDOUBLE, &ColorPicker::OnSpin, this);
	rowSizer->Add(*values.widget, 1, wxALIGN_CENTER, 5);

	sizer->Add(rowSizer, 1, wxEXPAND, 5);
}

void ColorPicker::AddGroup(
	wxBoxSizer* mainSizer,
	std::vector<SpinValues> controls)
{
	wxBoxSizer* groupSizer = new wxBoxSizer(wxVERTICAL);
	for (auto& control : controls) {
		AddSpin(groupSizer, control);
	}
	mainSizer->Add(groupSizer, 1, wxEXPAND, 0);
}

ColorPicker::ColorPicker(
	wxWindow* parent,
	double* gamma,
	const double (**M)[3][3],
	const double (**M_INV)[3][3]) :
	wxPanel(parent),
	gamma(gamma),
	M(M),
	M_INV(M_INV)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	colorLabel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	colorLabel->SetMinSize(wxSize(100,100));
	colorLabel->SetBackgroundColour(wxColour(0, 0, 0));

	mainSizer->Add(colorLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	AddGroup(mainSizer, {
		{ ID_HSV, &spinHsvHue, wxT("Hue"), 0, 359, 1, 0 },
		{ ID_HSV, &spinHsvSat, wxT("Saturation") },
		{ ID_HSV, &spinHsvVal, wxT("Value") }
	});

	AddGroup(mainSizer, {
		{ ID_RGB, &spinRgbRed, wxT("Red") },
		{ ID_RGB, &spinRgbGreen, wxT("Green") },
		{ ID_RGB, &spinRgbBlue, wxT("Blue") }
	});

	AddGroup(mainSizer, {
		{ ID_XYZ, &spinXyzX, wxT("X") },
		{ ID_XYZ, &spinXyzY, wxT("Y") },
		{ ID_XYZ, &spinXyzZ, wxT("Z") }
	});

	AddGroup(mainSizer, {
		{ ID_LUV, &spinLuvL, wxT("L"), 0, 100, 1, 0 },
		{ ID_LUV, &spinLuvU, wxT("U"), -200, 200, 1, 0 },
		{ ID_LUV, &spinLuvV, wxT("V"), -200, 200, 1, 0 }
	});

	AddGroup(mainSizer, {
		{ ID_LCH, &spinLchL, wxT("L"), 0, 100, 1, 0 },
		{ ID_LCH, &spinLchC, wxT("C"), 0, 200, 1, 0 },
		{ ID_LCH, &spinLchH, wxT("H"), 0, 359, 1, 0 }
	});

	this->SetSizer(mainSizer);
	this->Layout();
}

void ColorPicker::OnSpin(wxSpinDoubleEvent& event)
{
	switch (event.GetId())
	{
	case ID_RGB:
		UpdateRGB();
		break;
	case ID_HSV:
		UpdateHSV();
		break;
	case ID_XYZ:
		UpdateXYZ();
		break;
	case ID_LUV:
		UpdateLUV();
		break;
	case ID_LCH:
		UpdateLCH();
		break;
	default:
		break;
	}
}

namespace
{

bool ApproxEqual(const RGB& lhs, const RGB& rhs)
{
	return
		int(lhs.r * 100) == int(rhs.r * 100) &&
		int(lhs.g * 100) == int(rhs.g * 100) &&
		int(lhs.b * 100) == int(rhs.b * 100);
}

bool NotApproxEqual(const RGB& lhs, const RGB& rhs)
{
	return !ApproxEqual(lhs, rhs);
}

}

void ColorPicker::UpdateRGB()
{
	SetColor(CurrentRGB());
}

void ColorPicker::UpdateHSV()
{
	HSV hsv {
		spinHsvHue->GetValue(),
		spinHsvSat->GetValue(),
		spinHsvVal->GetValue()
	};
	auto newRGB = hsvToRGB(hsv);
	if (NotApproxEqual(newRGB, CurrentRGB()))
		SetColor(newRGB);
}

void ColorPicker::UpdateXYZ()
{
	XYZ xyz {
		spinXyzX->GetValue(),
		spinXyzY->GetValue(),
		spinXyzZ->GetValue()
	};
	auto newRGB = xyzToRGB(xyz, **M_INV, *gamma);
	if (NotApproxEqual(newRGB, CurrentRGB()))
		SetColor(newRGB);
}

void ColorPicker::UpdateLUV()
{
	LUV luv {
		spinLuvL->GetValue(),
		spinLuvU->GetValue(),
		spinLuvV->GetValue()
	};
	auto newRGB = xyzToRGB(luvToXYZ(luv), **M_INV, *gamma);
	if (NotApproxEqual(newRGB, CurrentRGB()))
		SetColor(newRGB);
}

void ColorPicker::UpdateLCH()
{
	LCH lch {
		spinLchL->GetValue(),
		spinLchC->GetValue(),
		spinLchH->GetValue()
	};
	auto newRGB = xyzToRGB(luvToXYZ(lchToLUV(lch)), **M_INV, *gamma);
	if (NotApproxEqual(newRGB, CurrentRGB()))
		SetColor(newRGB);
}

RGB ColorPicker::CurrentRGB()
{
	return RGB {
		spinRgbRed->GetValue(),
		spinRgbGreen->GetValue(),
		spinRgbBlue->GetValue()
	};
}

void ColorPicker::SetColor(const RGB& color)
{
	colorLabel->SetBackgroundColour(wxColour(color.r * 255, color.g * 255, color.b * 255));
	colorLabel->Refresh();

	// Every other space
	HSV hsv = rgbToHSV(color);
	XYZ xyz = rgbToXYZ(color, **M, *gamma);
	LUV luv = xyzToLUV(xyz);
	LCH lch = luvToLCH(luv);

	// Update widgets
	spinRgbRed->SetValue(color.r);
	spinRgbGreen->SetValue(color.g);
	spinRgbBlue->SetValue(color.b);

	spinHsvHue->SetValue(hsv.H);
	spinHsvSat->SetValue(hsv.S);
	spinHsvVal->SetValue(hsv.V);

	spinXyzX->SetValue(xyz.X);
	spinXyzY->SetValue(xyz.Y);
	spinXyzZ->SetValue(xyz.Z);

	spinLuvL->SetValue(luv.L);
	spinLuvU->SetValue(luv.U);
	spinLuvV->SetValue(luv.V);

	spinLchL->SetValue(lch.L);
	spinLchC->SetValue(lch.C);
	spinLchH->SetValue(lch.H);
}
