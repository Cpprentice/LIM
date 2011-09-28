#include "app.hpp"
#include "logframe.hpp"

IMPLEMENT_APP(App);

bool App::OnInit()
{
	Logframe* frame = new Logframe();
	frame->Show();
	return true;
}
