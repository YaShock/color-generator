#include "app_frame.h"

class ColorGeneratorApp : public wxApp
{
	virtual bool OnInit();
};

IMPLEMENT_APP(ColorGeneratorApp);

bool ColorGeneratorApp::OnInit()
{
	AppFrame* frame = new AppFrame("Color Generator", wxPoint(100, 100), wxSize(500, 500));
	frame->Show();
	SetTopWindow(frame);
	return true;
}
