#include "logframe.hpp"


Logframe::Logframe() : wxFrame( 0, wxID_ANY, "TCP CHAT SERVER", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE &~ (wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	SetClientSize(400,200);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	wxTextCtrl* txtctrl = new wxTextCtrl(this, ID_LOG, "", wxDefaultPosition, wxSize(400, 200), wxTE_MULTILINE|wxTE_READONLY );
	//txtctrl->Enable(false);
	sizer->Add(txtctrl, 1);
	SetSizer(sizer);


	wxLogTextCtrl *logWindow = new wxLogTextCtrl(GetLog());
    delete wxLog::SetActiveTarget(logWindow);
    wxLogMessage("Server Initialized");

    server = new TCPServer;
	if (server->Create("11111"))
    {
        wxLogMessage(_T("Server started on port %s"), "11111");
        wxLogMessage(_T("Server uses TCP"));
    }
    else
    {
        wxLogMessage(_T("The TCP server on port %s failed to start"), "11111");
        delete server;
        server = NULL;
    }
}

Logframe::~Logframe()
{
	delete server;
}
