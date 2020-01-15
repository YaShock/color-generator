
#ifndef APP_FRAME_H
#define APP_FRAME_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/spinctrl.h>

#include "palette_widget.h"

class AppFrame : public wxFrame
{
public:
	AppFrame(
		const wxString& title,
		const wxPoint& pos,
		const wxSize& size);
	void SetDefaultValues();
	void SetupUI();

	void OnSpin(wxSpinDoubleEvent&);

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

	PaletteWidget* paletteWidget;

	int numColors;
	double gamma;
	PaletteType paletteType;
	const double (*M)[3][3];
	const double (*M_INV)[3][3];
};

#endif // APP_FRAME_H
