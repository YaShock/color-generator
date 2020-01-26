#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/spinctrl.h>
#include "utils.h"

class ColorPicker : public wxPanel
{
public:
	ColorPicker(wxWindow* parent);
	void SetColor(const RGB& color);

private:
	wxStaticText* colorLabel;
	wxSpinCtrlDouble* spinHsvHue;
	wxSpinCtrlDouble* spinHsvSat;
	wxSpinCtrlDouble* spinHsvVal;
	wxSpinCtrlDouble* spinRgbRed;
	wxSpinCtrlDouble* spinRgbGreen;
	wxSpinCtrlDouble* spinRgbBlue;
	wxSpinCtrlDouble* spinXyzX;
	wxSpinCtrlDouble* spinXyzY;
	wxSpinCtrlDouble* spinXyzZ;
	wxSpinCtrlDouble* spinLuvL;
	wxSpinCtrlDouble* spinLuvU;
	wxSpinCtrlDouble* spinLuvV;
	wxSpinCtrlDouble* spinLchL;
	wxSpinCtrlDouble* spinLchC;
	wxSpinCtrlDouble* spinLchH;
};

#endif // COLOR_PICKER_H
