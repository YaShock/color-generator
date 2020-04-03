#include "app_frame.h"

#include "color_picker.h"
#include "utils.h"
#include "palette.h"

namespace
{

enum SliderIDs
{
	SLIDER_CONSTRAST = 100,
	SLIDER_SATURATION,
	SLIDER_BRIGHTNESS,
	SLIDER_COLD_WARM,
	SLIDER_HUE,
	SLIDER_HUE_PRIMARY,
	SLIDER_HUE_SECONDARY,
	SPIN_NUM_COLORS,
	SPIN_GAMMA,
	CHOICE_SPACE,
	CLICK_COLOR
};

void AddTextAndSlider(
	wxFrame* parent,
	wxFlexGridSizer* sizer,
	wxStaticText** text,
	wxSlider** slider,
	const wxString& label,
	int sliderID,
	int minValue = 0,
	int maxValue = 100)
{
	*text = new wxStaticText(parent, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
	(*text)->Wrap( -1 );
	sizer->Add(*text, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	*slider = new wxSlider(parent, sliderID, 0, minValue, maxValue, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL);
	sizer->Add(*slider, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
}

}

AppFrame::AppFrame(
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size)
	: wxFrame(nullptr, -1, title, pos, size, wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER)
{
	SetDefaultValues();
	SetupUI();
	BindControls();

	paletteWidget->SetColorCallback(
		[this](const RGB& color) {
			colorPicker->SetColor(color);
		});
}

void AppFrame::SetDefaultValues()
{
	numColors = 8;
	gamma = 2.2;
	paletteType = PaletteType::Sequential;
	M = &M_SRGB;
	M_INV = &M_INV_SRGB;
}

void AppFrame::SetupUI()
{
	SetSizeHints(wxDefaultSize, wxDefaultSize);

	mainSizer = new wxBoxSizer(wxVERTICAL);

	colorPicker = new ColorPicker(this, &gamma, &M, &M_INV);
	mainSizer->Add(colorPicker, 0, wxEXPAND, 5);

	wxBoxSizer* SizerGamma;
	SizerGamma = new wxBoxSizer(wxHORIZONTAL);

	stGamma = new wxStaticText(this, wxID_ANY, wxT("Gamma"), wxDefaultPosition, wxDefaultSize, 0);
	stGamma->Wrap(-1);
	SizerGamma->Add(stGamma, 0, wxALIGN_CENTER|wxALL, 5);

	spinGamma = new wxSpinCtrlDouble(this, SPIN_GAMMA, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 3, 1, 0.1);
	spinGamma->SetDigits(1);
	spinGamma->SetMinSize(wxSize(50, 30));
	SizerGamma->Add(spinGamma, 0, wxALIGN_CENTER|wxALL, 5);

	wxString ChoiceColorSpaceChoices[] = { wxT("SRGB"), wxT("AdobeRGB"), wxEmptyString };
	int ChoiceColorSpaceNChoices = sizeof( ChoiceColorSpaceChoices ) / sizeof( wxString );
	choiceColorSpace = new wxChoice(this, CHOICE_SPACE, wxDefaultPosition, wxDefaultSize, ChoiceColorSpaceNChoices, ChoiceColorSpaceChoices, 0);
	choiceColorSpace->SetSelection(0);
	SizerGamma->Add(choiceColorSpace, 0, wxALL, 5);


	mainSizer->Add(SizerGamma, 0, 0, 0);

	wxBoxSizer* SizerNumColors;
	SizerNumColors = new wxBoxSizer(wxHORIZONTAL);

	m_staticText16 = new wxStaticText(this, wxID_ANY, wxT("Number of colors"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText16->Wrap(-1);
	SizerNumColors->Add(m_staticText16, 0, wxALIGN_CENTER|wxALL, 5);

	spinNumColors = new wxSpinCtrlDouble(this, SPIN_NUM_COLORS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 20, 0, 1);
	spinNumColors->SetDigits(0);
	spinNumColors->Bind(wxEVT_SPINCTRLDOUBLE, &AppFrame::OnSpin, this);
	SizerNumColors->Add(spinNumColors, 0, wxALIGN_CENTER|wxALL, 5);
	spinNumColors->SetValue(numColors);


	mainSizer->Add(SizerNumColors, 0, wxEXPAND, 5);

	wxString ChoicePaletteTypeChoices[] = { wxT("Sequential"), wxT("Bivariate"), wxT("Qualitative") };
	int ChoicePaletteTypeNChoices = sizeof(ChoicePaletteTypeChoices) / sizeof(wxString);
	choicePaletteType = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoicePaletteTypeNChoices, ChoicePaletteTypeChoices, 0);
	choicePaletteType->SetSelection(0);
	choicePaletteType->Bind(wxEVT_CHOICE, &AppFrame::OnPaletteType, this);
	mainSizer->Add(choicePaletteType, 0, wxALL|wxEXPAND, 5);

	wxFlexGridSizer* tableControls;
	tableControls = new wxFlexGridSizer(10, 2, 0, 0);
	tableControls->AddGrowableCol(1);
	tableControls->SetFlexibleDirection(wxHORIZONTAL);
	tableControls->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	AddTextAndSlider(this, tableControls, &textContrast, &sliderContrast, wxT("Contrast:"), SLIDER_CONSTRAST);
	AddTextAndSlider(this, tableControls, &textSaturation, &sliderSaturation, wxT("Saturation:"), SLIDER_SATURATION);
	AddTextAndSlider(this, tableControls, &textBrightness, &sliderBrightness, wxT("Brightness:"), SLIDER_BRIGHTNESS);
	AddTextAndSlider(this, tableControls, &textColdWarm, &sliderColdWarm, wxT("Cold/Warm:"), SLIDER_COLD_WARM);
	AddTextAndSlider(this, tableControls, &textHue, &sliderHue, wxT("Hue:"), SLIDER_HUE, 0, 359);
	AddTextAndSlider(this, tableControls, &textHue0, &sliderHue0, wxT("Hue primary:"), SLIDER_HUE_PRIMARY, 0, 359);
	AddTextAndSlider(this, tableControls, &textHue1, &sliderHue1, wxT("Hue secondary:"), SLIDER_HUE_SECONDARY, 0, 359);
	AddTextAndSlider(this, tableControls, &textMidPoint, &sliderMidPoint, wxT("Midpoint:"), SLIDER_HUE);
	AddTextAndSlider(this, tableControls, &textHueRange, &sliderHueRange, wxT("Hue range:"), SLIDER_HUE);
	AddTextAndSlider(this, tableControls, &textHueShift, &sliderHueShift, wxT("Hue shift:"), SLIDER_HUE);

	// Default values
	sliderContrast->SetValue(int(88));
	sliderSaturation->SetValue(int(60));
	sliderBrightness->SetValue(int(75));
	sliderColdWarm->SetValue(int(15));
	sliderHue->SetValue(0);
	sliderHue0->SetValue(0);
	sliderHue1->SetValue(0);
	sliderMidPoint->SetValue(50);
	sliderHueRange->SetValue(0);
	sliderHueShift->SetValue(0);
	spinGamma->SetValue(gamma);

	mainSizer->Add(tableControls, 0, wxEXPAND, 5);

	textGenPalette = new wxStaticText(this, wxID_ANY, wxT("Generated palette"), wxDefaultPosition, wxDefaultSize, 0);
	textGenPalette->Wrap(-1);
	mainSizer->Add(textGenPalette, 0, wxALIGN_CENTER|wxALL, 5);

	widgetsSeq = {
		sliderHue,
		sliderSaturation,
		sliderContrast,
		sliderBrightness,
		sliderColdWarm,
		textHue,
		textSaturation,
		textContrast,
		textBrightness,
		textColdWarm
	};
	widgetsDiv = {
		sliderSaturation,
		sliderContrast,
		sliderBrightness,
		sliderColdWarm,
		sliderHue0,
		sliderHue1,
		sliderMidPoint,
		textSaturation,
		textContrast,
		textBrightness,
		textColdWarm,
		textHue0,
		textHue1,
		textMidPoint
	};

	widgetsQual = {
		sliderSaturation,
		sliderContrast,
		sliderBrightness,
		sliderHueRange,
		sliderHueShift,
		textSaturation,
		textContrast,
		textBrightness,
		textHueRange,
		textHueShift
	};

	SetPaletteTypeWidgets();
	SetupPalette();
	paletteWidget = new PaletteWidget(
		this,
		&numColors,
		&gamma,
		&M_INV,
		palette);

	mainSizer->Add(paletteWidget, 1, wxEXPAND|wxALL, 5);


	SetSizerAndFit(mainSizer);
	Layout();

	Centre(wxBOTH);
}

void AppFrame::BindControls()
{
	sliderContrast->Bind(wxEVT_SLIDER, &AppFrame::OnSlider, this);
	sliderSaturation->Bind(wxEVT_SLIDER, &AppFrame::OnSlider, this);
	sliderBrightness->Bind(wxEVT_SLIDER, &AppFrame::OnSlider, this);
	sliderColdWarm->Bind(wxEVT_SLIDER, &AppFrame::OnSlider, this);
	sliderHue->Bind(wxEVT_SLIDER, &AppFrame::OnSlider, this);
	sliderHue0->Bind(wxEVT_SLIDER, &AppFrame::OnSlider, this);
	sliderHue1->Bind(wxEVT_SLIDER, &AppFrame::OnSlider, this);
	sliderHueRange->Bind(wxEVT_SLIDER, &AppFrame::OnSlider, this);
	sliderHueShift->Bind(wxEVT_SLIDER, &AppFrame::OnSlider, this);
	sliderMidPoint->Bind(wxEVT_SLIDER, &AppFrame::OnSlider, this);
	spinGamma->Bind(wxEVT_SPINCTRLDOUBLE, &AppFrame::OnSpin, this);
	choiceColorSpace->Bind(wxEVT_CHOICE, &AppFrame::OnSlider, this);
}

void AppFrame::OnSpin(wxSpinDoubleEvent& event)
{
	switch (event.GetId())
	{
	case SPIN_NUM_COLORS:
		numColors = int(spinNumColors->GetValue());
		break;
	case SPIN_GAMMA:
		gamma = spinGamma->GetValue();
		break;
	default:
		break;
	}
	RefreshPalette();
}

void AppFrame::OnSlider(wxCommandEvent& event)
{
	int s = choiceColorSpace->GetCurrentSelection();
	switch (event.GetId())
	{
	case CHOICE_SPACE:
		if (choiceColorSpace->GetString(s) == wxT("SRGB"))
		{
			M = &M_SRGB;
			M_INV = &M_INV_SRGB;
		}
		else if (choiceColorSpace->GetString(s) == wxT("AdobeRGB"))
		{
			M = &M_ADOBE;
			M_INV = &M_INV_ADOBE;
		}
		break;
	default:
		break;
	}
	RefreshPalette();
}

void AppFrame::SetPaletteTypeWidgets()
{
	for (auto widget : widgetsSeq)
	{
		widget->Hide();
	}
	for (auto widget : widgetsDiv)
	{
		widget->Hide();
	}
	for (auto widget : widgetsQual)
	{
		widget->Hide();
	}

	//Activate only current scales and labels
	if (paletteType == PaletteType::Sequential)
	{
		for (auto widget : widgetsSeq)
		{
			widget->Show();
		}
	}
	else if (paletteType == PaletteType::Bivariate)
	{
		for (auto widget : widgetsDiv)
		{
			widget->Show();
		}
	}
	else if (paletteType == PaletteType::Qualitative)
	{
		for (auto widget : widgetsQual)
		{
			widget->Show();
		}
	}

}

void AppFrame::OnPaletteType(wxCommandEvent&)
{
	int s = choicePaletteType->GetCurrentSelection();

	if (choicePaletteType->GetString(s) == wxT("Sequential") &&
		paletteType != PaletteType::Sequential)
	{
		paletteType = PaletteType::Sequential;
		SetPaletteTypeWidgets();
		RefreshPalette();
	}
	else if (choicePaletteType->GetString(s) == wxT("Bivariate") &&
		paletteType != PaletteType::Bivariate)
	{
		paletteType = PaletteType::Bivariate;
		SetPaletteTypeWidgets();
		RefreshPalette();
	}
	else if (choicePaletteType->GetString(s) == wxT("Qualitative") &&
		paletteType != PaletteType::Qualitative)
	{
		paletteType = PaletteType::Qualitative;
		SetPaletteTypeWidgets();
		RefreshPalette();
	}
}

void AppFrame::RefreshPalette()
{
	auto oldSize = GetSize();

	SetupPalette();
	paletteWidget->GeneratePalette();
	SetSizerAndFit(mainSizer);
	Layout();

	//SetSize(
	//	std::max(GetSize().GetWidth(), oldSize.GetWidth()),
	//	std::max(GetSize().GetHeight(), oldSize.GetHeight()));
}

void AppFrame::SetupPalette()
{
	double hue = sliderHue->GetValue();
	double hue0 = sliderHue0->GetValue();
	double hue1 = sliderHue1->GetValue();
	double saturation = sliderSaturation->GetValue() / 100.0;
	double brightness = sliderBrightness->GetValue() / 100.0;
	double contrast = sliderContrast->GetValue() / 100.0;
	double coldWarm = sliderColdWarm->GetValue() / 100.0;
	double midPoint = sliderMidPoint->GetValue() / 100.0;
	double hueShift = sliderHueShift->GetValue() / 100.0;
	double hueRange = sliderHueRange->GetValue() / 100.0;
	switch (paletteType)
	{
	case PaletteType::Sequential:
		palette.reset(new SeqPalette(
			generateSeqPalette(
				*M,
				gamma,
				hue,
				saturation,
				brightness,
				contrast,
				coldWarm)));
		break;
	case PaletteType::Bivariate:
		palette.reset(new DivPalette(
			generateDivPalette(
				*M,
				gamma,
				hue0,
				hue1,
				saturation,
				brightness,
				contrast,
				coldWarm,
				midPoint)));
		break;
	case PaletteType::Qualitative:
		palette.reset(new QualPalette(
			generateQualPalette(
				*M,
				gamma,
				hueShift,
				hueRange,
				saturation,
				brightness,
				contrast)));
		break;
	default:
		break;
	}
}
