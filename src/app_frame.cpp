#include "app_frame.h"
#include "color_picker.h"
#include "palette.h"

AppFrame::AppFrame(
	const wxString& title,
	const wxPoint& pos,
	const wxSize& size)
	: wxFrame(nullptr, -1, title, pos, size)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	MainSizer = new wxBoxSizer( wxVERTICAL );

	ColorPicker* colorPicker = new ColorPicker(this);
	MainSizer->Add( colorPicker, 0, wxEXPAND, 5 );

	wxBoxSizer* SizerGamma;
	SizerGamma = new wxBoxSizer( wxHORIZONTAL );

	stGamma = new wxStaticText( this, wxID_ANY, wxT("Gamma"), wxDefaultPosition, wxDefaultSize, 0 );
	stGamma->Wrap( -1 );
	SizerGamma->Add( stGamma, 0, wxALIGN_CENTER|wxALL, 5 );

	spinGamma = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	spinGamma->SetDigits( 0 );
	SizerGamma->Add( spinGamma, 0, wxALIGN_CENTER|wxALL, 5 );

	wxString ChoiceColorSpaceChoices[] = { wxT("SRGB"), wxT("AdobeRGB"), wxEmptyString };
	int ChoiceColorSpaceNChoices = sizeof( ChoiceColorSpaceChoices ) / sizeof( wxString );
	ChoiceColorSpace = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoiceColorSpaceNChoices, ChoiceColorSpaceChoices, 0 );
	ChoiceColorSpace->SetSelection( 0 );
	SizerGamma->Add( ChoiceColorSpace, 0, wxALL, 5 );


	MainSizer->Add( SizerGamma, 0, 0, 0 );

	wxBoxSizer* SizerNumColors;
	SizerNumColors = new wxBoxSizer( wxHORIZONTAL );

	m_staticText16 = new wxStaticText( this, wxID_ANY, wxT("Number of colors"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	SizerNumColors->Add( m_staticText16, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble2 = new wxSpinCtrlDouble( this, 1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 20, 0, 1 );
	m_spinCtrlDouble2->SetDigits( 0 );
	m_spinCtrlDouble2->Bind(wxEVT_COMMAND_SPINCTRL_UPDATED, &AppFrame::OnSpin, this);
	SizerNumColors->Add( m_spinCtrlDouble2, 0, wxALIGN_CENTER|wxALL, 5 );


	MainSizer->Add( SizerNumColors, 0, wxEXPAND, 5 );

	wxString ChoicePaletteTypeChoices[] = { wxT("Sequential"), wxT("Bivariate"), wxT("Qualitative"), wxEmptyString };
	int ChoicePaletteTypeNChoices = sizeof( ChoicePaletteTypeChoices ) / sizeof( wxString );
	ChoicePaletteType = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoicePaletteTypeNChoices, ChoicePaletteTypeChoices, 0 );
	ChoicePaletteType->SetSelection( 0 );
	MainSizer->Add( ChoicePaletteType, 0, wxALL|wxEXPAND, 5 );

	wxFlexGridSizer* TableControls;
	TableControls = new wxFlexGridSizer( 5, 2, 0, 0 );
	TableControls->AddGrowableCol( 1 );
	TableControls->SetFlexibleDirection( wxHORIZONTAL );
	TableControls->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText24 = new wxStaticText( this, wxID_ANY, wxT("Contrast:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	TableControls->Add( m_staticText24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_slider7 = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	TableControls->Add( m_slider7, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_staticText241 = new wxStaticText( this, wxID_ANY, wxT("Saturation:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText241->Wrap( -1 );
	TableControls->Add( m_staticText241, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_slider71 = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	TableControls->Add( m_slider71, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_staticText242 = new wxStaticText( this, wxID_ANY, wxT("Brightness:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText242->Wrap( -1 );
	TableControls->Add( m_staticText242, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_slider711 = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	TableControls->Add( m_slider711, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_staticText2421 = new wxStaticText( this, wxID_ANY, wxT("Cold/Warm:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2421->Wrap( -1 );
	TableControls->Add( m_staticText2421, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_slider7111 = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	TableControls->Add( m_slider7111, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_staticText24211 = new wxStaticText( this, wxID_ANY, wxT("Hue:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24211->Wrap( -1 );
	TableControls->Add( m_staticText24211, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_slider71111 = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	TableControls->Add( m_slider71111, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );


	MainSizer->Add( TableControls, 0, wxEXPAND, 5 );

	ButtonGenerate = new wxButton(this, wxID_ANY, wxT("Generate"));
	MainSizer->Add(ButtonGenerate, 0, wxALIGN_CENTER|wxALL, 5);
	ButtonGenerate->Bind(wxEVT_BUTTON, &AppFrame::OnGenerate, this);

	TextGenPalette = new wxStaticText( this, wxID_ANY, wxT("Generated palette"), wxDefaultPosition, wxDefaultSize, 0 );
	TextGenPalette->Wrap( -1 );
	MainSizer->Add( TextGenPalette, 0, wxALIGN_CENTER|wxALL, 5 );


	this->SetSizerAndFit(MainSizer);
	this->Layout();

	this->Centre(wxBOTH);
}

void AppFrame::OnSpin(wxSpinEvent&)
{
	std::cout << "OnSpin\n";
	// int noColors = int(m_spinCtrlDouble2->GetValue());
	// for (int i = 0; i < noColors; ++i)
	// {
	// 	auto colorLabel = new wxPanel(this);
	// 	colorLabel->SetBackgroundColour(*wxRED);
	// 	colorLabel->SetMinSize(wxSize(100, 10));
	// 	MainSizer->Add(colorLabel, 1, wxEXPAND|wxALL, 5);
	// 	SetSizerAndFit(MainSizer);
	// }
}

void AppFrame::OnGenerate(wxCommandEvent&)
{
	int N = int(m_spinCtrlDouble2->GetValue());

	double c = 0.88 < 0.34 + 0.06 * N ? 0.88 : 0.06 * N;
	auto p = generateSeqPalette(
		M_SRGB,
		2.2,
		150,
		0.6,
		0.75,
		c,
		0.15);
	auto colors = generateColors(p, N);

	for (int i = 0; i < N; ++i)
	{
		auto colorLabel = new wxPanel(this);
		RGB color = xyzToRGB(luvToXYZ(lchToLUV(colors[i])), M_INV_SRGB, 2.2);
		colorLabel->SetBackgroundColour(wxColour(color.r * 255, color.g * 255, color.b * 255));
		colorLabel->SetMinSize(wxSize(100, 10));
		MainSizer->Add(colorLabel, 1, wxEXPAND|wxALL, 5);
		SetSizerAndFit(MainSizer);
	}
}
