#ifndef LOGFRAME_HPP
#define LOGFRAME_HPP

#include <wx/wx.h>
#include "tcp.hpp"

#define ID_LOG 1234

class Logframe : public wxFrame
{
	public:
		Logframe();
		~Logframe();
		wxTextCtrl* GetLog()  { return (wxTextCtrl*) FindWindow( ID_LOG ); }
	private:
		TCPServer* server;
};

#endif
