#include "color_picker.h"

ColorPicker::ColorPicker(wxWindow* parent)
	: wxPanel(parent)
{
	wxBoxSizer* PickerSizer;
	PickerSizer = new wxBoxSizer( wxHORIZONTAL );

	m_bitmap1 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	m_bitmap1->SetMinSize( wxSize( 100,100 ) );

	PickerSizer->Add( m_bitmap1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxBoxSizer* Picker1;
	Picker1 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Hue"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer14->Add( m_staticText1, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble1 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble1->SetDigits( 0 );
	bSizer14->Add( m_spinCtrlDouble1, 1, wxALIGN_CENTER, 5 );


	Picker1->Add( bSizer14, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText13 = new wxStaticText( this, wxID_ANY, wxT("Saturation"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer141->Add( m_staticText13, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble11 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble11->SetDigits( 0 );
	bSizer141->Add( m_spinCtrlDouble11, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker1->Add( bSizer141, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer142;
	bSizer142 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText14 = new wxStaticText( this, wxID_ANY, wxT("Value"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer142->Add( m_staticText14, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble12 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble12->SetDigits( 0 );
	bSizer142->Add( m_spinCtrlDouble12, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker1->Add( bSizer142, 1, wxEXPAND, 5 );


	PickerSizer->Add( Picker1, 1, wxEXPAND, 0 );

	wxBoxSizer* Picker2;
	Picker2 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer143;
	bSizer143 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Red"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer143->Add( m_staticText11, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble13 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble13->SetDigits( 0 );
	bSizer143->Add( m_spinCtrlDouble13, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker2->Add( bSizer143, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1411;
	bSizer1411 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText131 = new wxStaticText( this, wxID_ANY, wxT("Green"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText131->Wrap( -1 );
	bSizer1411->Add( m_staticText131, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble111 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble111->SetDigits( 0 );
	bSizer1411->Add( m_spinCtrlDouble111, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker2->Add( bSizer1411, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1421;
	bSizer1421 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText141 = new wxStaticText( this, wxID_ANY, wxT("Blue"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText141->Wrap( -1 );
	bSizer1421->Add( m_staticText141, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble121 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble121->SetDigits( 0 );
	bSizer1421->Add( m_spinCtrlDouble121, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker2->Add( bSizer1421, 1, wxEXPAND, 5 );


	PickerSizer->Add( Picker2, 1, wxEXPAND, 5 );

	wxBoxSizer* Picker3;
	Picker3 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer144;
	bSizer144 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer144->Add( m_staticText12, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble14 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble14->SetDigits( 0 );
	bSizer144->Add( m_spinCtrlDouble14, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker3->Add( bSizer144, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1412;
	bSizer1412 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText132 = new wxStaticText( this, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText132->Wrap( -1 );
	bSizer1412->Add( m_staticText132, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble112 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble112->SetDigits( 0 );
	bSizer1412->Add( m_spinCtrlDouble112, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker3->Add( bSizer1412, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer1422;
	bSizer1422 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText142 = new wxStaticText( this, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText142->Wrap( -1 );
	bSizer1422->Add( m_staticText142, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble122 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble122->SetDigits( 0 );
	bSizer1422->Add( m_spinCtrlDouble122, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker3->Add( bSizer1422, 1, wxEXPAND, 5 );


	PickerSizer->Add( Picker3, 1, wxEXPAND, 5 );

	wxBoxSizer* Picker4;
	Picker4 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer1441;
	bSizer1441 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText121 = new wxStaticText( this, wxID_ANY, wxT("L"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	bSizer1441->Add( m_staticText121, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble141 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble141->SetDigits( 0 );
	bSizer1441->Add( m_spinCtrlDouble141, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker4->Add( bSizer1441, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer14121;
	bSizer14121 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1321 = new wxStaticText( this, wxID_ANY, wxT("U"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1321->Wrap( -1 );
	bSizer14121->Add( m_staticText1321, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble1121 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble1121->SetDigits( 0 );
	bSizer14121->Add( m_spinCtrlDouble1121, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker4->Add( bSizer14121, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer14221;
	bSizer14221 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1421 = new wxStaticText( this, wxID_ANY, wxT("V"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1421->Wrap( -1 );
	bSizer14221->Add( m_staticText1421, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble1221 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble1221->SetDigits( 0 );
	bSizer14221->Add( m_spinCtrlDouble1221, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker4->Add( bSizer14221, 1, wxEXPAND, 5 );


	PickerSizer->Add( Picker4, 1, wxEXPAND, 5 );

	wxBoxSizer* Picker5;
	Picker5 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer1442;
	bSizer1442 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText122 = new wxStaticText( this, wxID_ANY, wxT("L"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText122->Wrap( -1 );
	bSizer1442->Add( m_staticText122, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble142 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble142->SetDigits( 0 );
	bSizer1442->Add( m_spinCtrlDouble142, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker5->Add( bSizer1442, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer14122;
	bSizer14122 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1322 = new wxStaticText( this, wxID_ANY, wxT("C"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1322->Wrap( -1 );
	bSizer14122->Add( m_staticText1322, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble1122 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0, 1 );
	m_spinCtrlDouble1122->SetDigits( 0 );
	bSizer14122->Add( m_spinCtrlDouble1122, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker5->Add( bSizer14122, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer14222;
	bSizer14222 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1422 = new wxStaticText( this, wxID_ANY, wxT("H"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1422->Wrap( -1 );
	bSizer14222->Add( m_staticText1422, 0, wxALIGN_CENTER|wxALL, 5 );

	m_spinCtrlDouble1222 = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0.000000, 1 );
	m_spinCtrlDouble1222->SetDigits( 0 );
	bSizer14222->Add( m_spinCtrlDouble1222, 1, wxALIGN_CENTER|wxALL, 5 );


	Picker5->Add( bSizer14222, 1, wxEXPAND, 5 );


	PickerSizer->Add( Picker5, 1, wxEXPAND, 5 );

	this->SetSizer(PickerSizer);
	this->Layout();
}
