#pragma once

#define HELLO_PORT_STR "50013"
#define SERVER_IP "localhost" //La ip privada que corresponde a si mismo, boost entiende localhost como ip propia
//En vez de localhost se puede reemplazar por "127.0.0.1"
#define MAX_MESSAGE_LENGTH 295


class client
{
	char * host_arg;

public:
	client(char * arg);
	void startConnection(const char* host);
	void receiveMessage();
	void addPathToMessage(char * path);
	const char* getFinalMessage();
	


	~client();

private:
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* socket_forClient;		//Si tengo N servers, tengo N sockets
	boost::asio::ip::tcp::resolver* client_resolver;	//Los dos de siguientes se usar para "resolver" el server
	boost::asio::ip::tcp::resolver::iterator endpoint;
	const char * finalMessage;
};