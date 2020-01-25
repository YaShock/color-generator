#include "app_frame.h"

#include "color_picker.h"
#include "utils.h"

namespace
{

enum SliderIDs
{
	SLIDER_CONSTRAST = 100,
	SLIDER_SATURATION,
	SLIDER_BRIGHTNESS,
	SLIDER_COLD_WARM,
	SLIDER_HUE,
	SPIN_NUM_COLORS,
	SPIN_GAMMA,
	CHOICE_SPACE
};

void AddTextAndSlider(
	wxFrame* parent,
	wxFlexGridSizer* sizer,
	wxSlider** slider,
	const wxString& label,
	int sliderID,
	int minValue = 0,
	int maxValue = 100)
{
	auto staticText = new wxStaticText(parent, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
	staticText->Wrap( -1 );
	sizer->Add(staticText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	*slider = new wxSlider(parent, sliderID, 0, minValue, maxValue, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL);
	sizer->Add(*slider, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
}

}

AppFrame::AppFrame(
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size)
	: wxFrame(nullptr, -1, title, pos, size)
{
	SetDefaultValues();
	SetupUI();
	BindControls();
}

void AppFrame::SetDefaultValues()
{
	contrast = 0.88;
	saturation = 0.6;
	brightness = 0.75;
	coldWarm = 0.15;
	hue = 0;

	numColors = 8;
	gamma = 2.2;
	paletteType = PaletteType::Sequential;
	M = &M_SRGB;
	M_INV = &M_INV_SRGB;
}

void AppFrame::SetupUI()
{
	SetSizeHints(wxDefaultSize, wxDefaultSize);

	MainSizer = new wxBoxSizer(wxVERTICAL);

	ColorPicker* colorPicker = new ColorPicker(this);
	MainSizer->Add(colorPicker, 0, wxEXPAND, 5);

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
	ChoiceColorSpace = new wxChoice(this, CHOICE_SPACE, wxDefaultPosition, wxDefaultSize, ChoiceColorSpaceNChoices, ChoiceColorSpaceChoices, 0);
	ChoiceColorSpace->SetSelection(0);
	SizerGamma->Add(ChoiceColorSpace, 0, wxALL, 5);


	MainSizer->Add(SizerGamma, 0, 0, 0);

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


	MainSizer->Add(SizerNumColors, 0, wxEXPAND, 5);

	wxString ChoicePaletteTypeChoices[] = { wxT("Sequential"), wxT("Bivariate"), wxT("Qualitative"), wxEmptyString };
	int ChoicePaletteTypeNChoices = sizeof(ChoicePaletteTypeChoices) / sizeof(wxString);
	ChoicePaletteType = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoicePaletteTypeNChoices, ChoicePaletteTypeChoices, 0);
	ChoicePaletteType->SetSelection(0);
	MainSizer->Add(ChoicePaletteType, 0, wxALL|wxEXPAND, 5);

	wxFlexGridSizer* tableControls;
	tableControls = new wxFlexGridSizer(5, 2, 0, 0);
	tableControls->AddGrowableCol(1);
	tableControls->SetFlexibleDirection(wxHORIZONTAL);
	tableControls->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	AddTextAndSlider(this, tableControls, &sliderContrast, wxT("Contrast:"), SLIDER_CONSTRAST);
	AddTextAndSlider(this, tableControls, &sliderSaturation, wxT("Saturation:"), SLIDER_SATURATION);
	AddTextAndSlider(this, tableControls, &sliderBrightness, wxT("Brightness:"), SLIDER_BRIGHTNESS);
	AddTextAndSlider(this, tableControls, &sliderColdWarm, wxT("Cold/Warm:"), SLIDER_COLD_WARM);
	AddTextAndSlider(this, tableControls, &sliderHue, wxT("Hue:"), SLIDER_HUE, 0, 359);

	sliderContrast->SetValue(int(contrast * 100));
	sliderSaturation->SetValue(int(saturation * 100));
	sliderBrightness->SetValue(int(brightness * 100));
	sliderColdWarm->SetValue(int(coldWarm * 100));
	sliderHue->SetValue(hue);
	spinGamma->SetValue(gamma);

	MainSizer->Add(tableControls, 0, wxEXPAND, 5);

	TextGenPalette = new wxStaticText(this, wxID_ANY, wxT("Generated palette"), wxDefaultPosition, wxDefaultSize, 0);
	TextGenPalette->Wrap(-1);
	MainSizer->Add(TextGenPalette, 0, wxALIGN_CENTER|wxALL, 5);

	paletteWidget = new PaletteWidget(
		this,
		&numColors,
		&gamma,
		&paletteType,
		&M,
		&M_INV,
		&contrast,
		&saturation,
		&brightness,
		&coldWarm,
		&hue);

	MainSizer->Add(paletteWidget, 1, wxEXPAND|wxALL, 5);


	SetSizerAndFit(MainSizer);
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
	spinGamma->Bind(wxEVT_SPINCTRLDOUBLE, &AppFrame::OnSpin, this);
	ChoiceColorSpace->Bind(wxEVT_CHOICE, &AppFrame::OnSlider, this);
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
	int s = ChoiceColorSpace->GetCurrentSelection();
	switch (event.GetId())
	{
	case SLIDER_CONSTRAST:
		contrast = sliderContrast->GetValue() / 100.0;
		break;
	case SLIDER_SATURATION:
		saturation = sliderSaturation->GetValue() / 100.0;
		break;
	case SLIDER_BRIGHTNESS:
		brightness = sliderBrightness->GetValue() / 100.0;
		break;
	case SLIDER_COLD_WARM:
		coldWarm = sliderColdWarm->GetValue() / 100.0;
		break;
	case SLIDER_HUE:
		hue = sliderHue->GetValue();
		break;
	case CHOICE_SPACE:
		if (ChoiceColorSpace->GetString(s) == wxT("SRGB"))
		{
			M = &M_SRGB;
			M_INV = &M_INV_SRGB;
		}
		else if (ChoiceColorSpace->GetString(s) == wxT("AdobeRGB"))
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

void AppFrame::RefreshPalette()
{
	auto oldSize = GetSize();

	paletteWidget->GeneratePalette();
	SetSizerAndFit(MainSizer);
	Layout();

	SetSize(
		std::max(GetSize().GetWidth(), oldSize.GetWidth()),
		std::max(GetSize().GetHeight(), oldSize.GetHeight()));
}
