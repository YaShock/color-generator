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
	enum CommandID
	{
		ID_RGB,
		ID_HSV,
		ID_XYZ,
		ID_LUV,
		ID_LCH
	};

	struct SpinValues
	{
		SpinValues(
			CommandID id,
			wxSpinCtrlDouble** widget,
			wxString name,
			int min = 0,
			int max = 1,
			double inc = 0.01,
			int digits = 2) :
			id(id),
			widget(widget),
			name(name),
			min(min),
			max(max),
			inc(inc),
			digits(digits)
		{
		}

		CommandID id;
		wxSpinCtrlDouble** widget;
		wxString name;
		int min;
		int max;
		double inc;
		int digits;
	};

public:
	ColorPicker(
		wxWindow* parent,
		double* gamma,
		const double (**M)[3][3],
		const double (**M_INV)[3][3]);
	void AddGroup(
		wxBoxSizer* mainSizer,
		std::vector<SpinValues> controls);
	void AddSpin(
		wxBoxSizer* sizer,
		const SpinValues& values);
	RGB CurrentRGB();
	void SetColor(const RGB& color);
	void UpdateRGB();
	void UpdateHSV();
	void UpdateXYZ();
	void UpdateLUV();
	void UpdateLCH();
	void OnSpin(wxSpinDoubleEvent& event);

private:
	wxPanel* colorLabel;
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

	double* gamma;
	const double (**M)[3][3];
	const double (**M_INV)[3][3];
};

#endif // COLOR_PICKER_H
