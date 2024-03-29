#include "tcp.hpp"



TCPServer::~TCPServer()
{
	for( size_t n = 0; n < conns.size(); n++)
	{
		conns[n]->Disconnect();
		delete conns[n];
	}
	conns.clear();
}

wxConnectionBase *TCPServer::OnAcceptConnection(const wxString& topic)
{
    wxLogMessage(_T("OnAcceptConnection(\"%s\")"), topic.c_str());

    wxString auth = topic.substr(0, 5);
    wxString name = topic.substr(5,topic.size()-5);

    if ( auth == "Logon" )
    {
    	TCPConnection* conn = new TCPConnection(this, name);
        conns.push_back(conn);
        wxLogMessage(_T("Connection to %s accepted"), name.c_str());

        //wxMessageBox(wxString::Format("%x", conn), "ptr_info");

		ConnecTimer* t = new ConnecTimer(this, name);
		t->Start(10, true);
        //wxString msg = name + " connected.";
        //conn->Advise("Logon", (char*)msg.c_str(), msg.size());

        return conn;
    }
    // unknown topic
    wxLogMessage("Connection denied!");
    return NULL;
}

size_t TCPServer::IDManager()
{
	size_t id = 0;
	bool valid = false;
	while( !valid)
	{
		valid = true;
		for( size_t n = 0; n < conns.size(); n++)
		{
			if( conns[n]->id == id)
			{
				valid = false;
				id++;
			}
		}
	}
	return id;
}

TCPConnection::TCPConnection(TCPServer* _server, wxString _nick) : wxTCPConnection()
{
	server = _server;
	id = server->IDManager();
	nick = _nick;
}

bool TCPConnection::OnDisconnect()
{
	wxLogMessage(_T("OnDisconnect()"));
	wxString nick;
	for( size_t n = 0; n < server->conns.size(); n++)
	{
		if( server->conns[n]->id == id)
		{
			nick = server->conns[n]->nick;
			delete server->conns[n];
			server->conns.erase(server->conns.begin()+n);
			wxString msg = "[";
			msg += wxDateTime::Now().FormatTime() + "] " + nick + " disconnected.";
			for( size_t i = 0; i < server->conns.size(); i++)
			{
				server->conns[i]->Advise("logoff", (char*)msg.c_str(), msg.size());
			}
			break;
		}
	}
	return true;
}

bool TCPConnection::OnPoke(const wxString& topic, const wxString& item, wxChar *data, int size, wxIPCFormat format)
{
	wxLogMessage("OnPoke with item %s", item.c_str());
	char* msg = new char[size+1];
	memcpy(msg, data, size);
	msg[size] = 0;
	wxLogMessage("The data was %s", msg);
	for( size_t i = 0; i < server->conns.size(); i++)
	{
		server->conns[i]->Advise("message", msg, size+1);
	}
	return true;
}

char* TCPConnection::OnRequest(const wxString& topic, const wxString& item, int *size, wxIPCFormat format)
{
	if( item == "nicklist")
	{
		wxString nicks = "";
		for( size_t i = 0; i < server->conns.size(); i++)
		{
			nicks += server->conns[i]->nick + "\n";
		}
		return (char*)nicks.c_str();
	}
	return 0;
}

ConnecTimer::ConnecTimer(TCPServer* _serv, wxString _nick)
{
	serv = _serv;
	nick = _nick;
}

void ConnecTimer::Notify()
{
	wxString msg = "[";
	msg += wxDateTime::Now().FormatTime() + "] " + nick + " connected.";
	for( size_t i = 0; i < serv->conns.size(); i++)
	{
		serv->conns[i]->Advise("logon", (char*)msg.c_str(), msg.size());
	}
}
