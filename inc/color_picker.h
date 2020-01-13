#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/spinctrl.h>

class ColorPicker : public wxPanel
{
public:
	ColorPicker(wxWindow* parent);

private:
	wxStaticBitmap* m_bitmap1;
	wxStaticText* m_staticText1;
	wxSpinCtrlDouble* m_spinCtrlDouble1;
	wxStaticText* m_staticText13;
	wxSpinCtrlDouble* m_spinCtrlDouble11;
	wxStaticText* m_staticText14;
	wxSpinCtrlDouble* m_spinCtrlDouble12;
	wxStaticText* m_staticText11;
	wxSpinCtrlDouble* m_spinCtrlDouble13;
	wxStaticText* m_staticText131;
	wxSpinCtrlDouble* m_spinCtrlDouble111;
	wxStaticText* m_staticText141;
	wxSpinCtrlDouble* m_spinCtrlDouble121;
	wxStaticText* m_staticText12;
	wxSpinCtrlDouble* m_spinCtrlDouble14;
	wxStaticText* m_staticText132;
	wxSpinCtrlDouble* m_spinCtrlDouble112;
	wxStaticText* m_staticText142;
	wxSpinCtrlDouble* m_spinCtrlDouble122;
	wxStaticText* m_staticText121;
	wxSpinCtrlDouble* m_spinCtrlDouble141;
	wxStaticText* m_staticText1321;
	wxSpinCtrlDouble* m_spinCtrlDouble1121;
	wxStaticText* m_staticText1421;
	wxSpinCtrlDouble* m_spinCtrlDouble1221;
	wxStaticText* m_staticText122;
	wxSpinCtrlDouble* m_spinCtrlDouble142;
	wxStaticText* m_staticText1322;
	wxSpinCtrlDouble* m_spinCtrlDouble1122;
	wxStaticText* m_staticText1422;
	wxSpinCtrlDouble* m_spinCtrlDouble1222;
};

#endif // COLOR_PICKER_H
