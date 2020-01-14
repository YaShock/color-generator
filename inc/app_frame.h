
#ifndef APP_FRAME_H
#define APP_FRAME_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/spinctrl.h>

class AppFrame : public wxFrame
{
public:
	AppFrame(
		const wxString& title,
		const wxPoint& pos,
		const wxSize& size);

	void OnSpin(wxSpinEvent&);
	void OnGenerate(wxCommandEvent&);

private:
	wxBoxSizer* MainSizer;
	wxStaticText* stGamma;
	wxSpinCtrlDouble* spinGamma;
	wxChoice* ChoiceColorSpace;
	wxStaticText* m_staticText16;
	wxSpinCtrlDouble* m_spinCtrlDouble2;
	wxChoice* ChoicePaletteType;
	wxStaticText* m_staticText24;
	wxSlider* m_slider7;
	wxStaticText* m_staticText241;
	wxSlider* m_slider71;
	wxStaticText* m_staticText242;
	wxSlider* m_slider711;
	wxStaticText* m_staticText2421;
	wxSlider* m_slider7111;
	wxStaticText* m_staticText24211;
	wxSlider* m_slider71111;
	wxStaticText* TextGenPalette;
	wxButton* ButtonGenerate;
};

#endif // APP_FRAME_H
