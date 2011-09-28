#ifndef TCP_HPP
#define TCP_HPP

#include <wx/wx.h>
#include <wx/sckipc.h>
#include <vector>

using namespace std;

class TCPServer;
class TCPConnection;
class ConnecTimer;

class TCPServer : public wxTCPServer
{
	friend class ConnecTimer;
	friend class TCPConnection;
	public:
		~TCPServer();
		wxConnectionBase* OnAcceptConnection(const wxString& topic);
		size_t IDManager();
	private:
		vector<TCPConnection*> conns;
};

class TCPConnection : public wxTCPConnection
{
	friend class TCPServer;
	public:
		TCPConnection(TCPServer* _server, wxString _nick);
		bool OnDisconnect();
		bool OnPoke(const wxString& topic, const wxString& item, wxChar *data, int size, wxIPCFormat format);
		char* OnRequest(const wxString& topic, const wxString& item, int *size, wxIPCFormat format);

	private:
		size_t id;
		wxString nick;
		TCPServer* server;
};

class ConnecTimer : public wxTimer
{
	public:
		void Notify();
		ConnecTimer(TCPServer* _serv, wxString _nick);

	private:
		wxString nick;
		TCPServer* serv;
};



#endif
