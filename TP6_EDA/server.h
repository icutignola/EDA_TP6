#pragma once
#include <iostream>
#include <cstring>

#define MESSAGE_LONG	550
#define PORT_CONECT	50013
#define NUMBER_OF_PATHS 3
#define SERVER_IP "localhost"


class server
{
public:
	server();
	void startConnection();
	bool recivedMessage();
	bool verifyMessage();
	void sendMessage();
	void loadsuccessMessage(void);
	void loadErrorMessage(void);
	

	~server(); // destroyer

private:
	boost::asio::io_service*  IO_handler;
	boost::asio::ip::tcp::socket* socketServer;
	boost::asio::ip::tcp::acceptor* server_acceptor;
	char inputMessage[MESSAGE_LONG];
	const char *outputMessage;
	const char * path[NUMBER_OF_PATHS] = { "mypath1/folder/folder","mypath2/folder/folder", "mypath3/folder/folder" };
	unsigned int lenInputMessage;
	FILE * serverFile;
	string takeFileValue(void);
};
