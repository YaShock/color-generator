
#ifndef APP_FRAME_H
#define APP_FRAME_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/spinctrl.h>

#include "palette_widget.h"

class ColorPicker;

class AppFrame : public wxFrame
{
public:
	AppFrame(
		const wxString& title,
		const wxPoint& pos,
		const wxSize& size);
	void SetDefaultValues();
	void SetupUI();
	void BindControls();

	void OnSpin(wxSpinDoubleEvent&);
	void OnSlider(wxCommandEvent&);
	void OnPaletteType(wxCommandEvent&);
	void RefreshPalette();

private:
	wxBoxSizer* mainSizer;
	ColorPicker* colorPicker;
	wxStaticText* stGamma;
	wxSpinCtrlDouble* spinGamma;
	wxChoice* choiceColorSpace;
	wxStaticText* m_staticText16;
	wxSpinCtrlDouble* spinNumColors;
	wxChoice* choicePaletteType;
	wxSlider* sliderContrast;
	wxSlider* sliderSaturation;
	wxSlider* sliderBrightness;
	wxSlider* sliderColdWarm;
	wxSlider* sliderHue;
	wxSlider* sliderHue0;
	wxSlider* sliderHue1;
	wxSlider* sliderMidPoint;
	wxSlider* sliderHueRange;
	wxSlider* sliderHueShift;
	wxStaticText* textGenPalette;
	wxButton* buttonGenerate;

	std::vector<wxControl*> widgetsSeq;
	std::vector<wxControl*> widgetsDiv;
	std::vector<wxControl*> widgetsQual;

	PaletteWidget* paletteWidget;

	// sequential palette
	double contrast;
	double saturation;
	double brightness;
	double coldWarm;
	int hue;

	int numColors;
	double gamma;
	PaletteType paletteType;
	const double (*M)[3][3];
	const double (*M_INV)[3][3];
};

#endif // APP_FRAME_H
