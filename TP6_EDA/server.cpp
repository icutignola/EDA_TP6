#include <iostream>
#include <cstdio>
#include <cstring>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/timer/timer.hpp>
#include "server.h"


using namespace std;

//******************************  Constructor server  ******************************************
server::server()
{
	IO_handler = new boost::asio::io_service();
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), PORT_CONECT);
	socketServer = new boost::asio::ip::tcp::socket(*IO_handler);				//Crea un socket vacio
	server_acceptor = new boost::asio::ip::tcp::acceptor(*IO_handler, ep);		//El server "empieza a escuchar"
}
//**********************************************************************************************

//******************************  startConection  **********************************************
void server:: startConnection()
{
	server_acceptor->non_blocking(true);
	boost::system::error_code errorServer;
	
	//El server se queda "observando" si alguien se conecto
	do 
	{
		server_acceptor->accept(*socketServer, errorServer);	//Si alguien se conecto lo cargo en socket

	} while ((errorServer.value() == WSAEWOULDBLOCK));
	if (errorServer)
	{
		cout << "Error mientras se intento conectar el server" << PORT_CONECT << "Port " << errorServer.message() << endl;
	}
	//server_acceptor->accept(*socket_forServer);
	socketServer->non_blocking(true);
}
//*********************************************************************************************

//********************************  recivedMessage  *******************************************
bool server::recivedMessage()
{
	boost::system::error_code error;
	bool answer = false;
	lenInputMessage = 0;

	double waitSeconds = 0.0;				// Tiempo que espero
	boost::timer::cpu_timer executeTime;	// Tiempo de ejecucion
	boost::timer::cpu_times pastTime;		// Tiempo que paso
	boost::timer::cpu_times currentTime;	// Tiempo real
	
	executeTime.start();

	pastTime = executeTime.elapsed();		// Guarda el tiempo que paso
	
	cout << "Recibiendo mensaje del puerto" << PORT_CONECT <<endl;

	do
	{
		// Se lee el socket, se carga la informacion en messeageRecived y se carga el largo del mensaje
		lenInputMessage = socketServer->read_some(boost::asio::buffer(inputMessage), error); 

		currentTime = executeTime.elapsed();

		if ((currentTime.wall - pastTime.wall) > 1e9)// 1 x 10^9
		{
			waitSeconds += (currentTime.wall - pastTime.wall) / 1e9;
			pastTime = currentTime;
			cout << "Pasaron " << (int)floor(waitSeconds) << " segundos." << endl; 
		}

		if (!error)
		{
			inputMessage[lenInputMessage] = '\0';
		}

	} while (error.value() == WSAEWOULDBLOCK);

	if (!error)
	{
		answer = true;
	}
	else
	{
		answer = false;
		cout << "Error mientras se intento cargar el mensaje" << error.message() << std::endl;
	}

	return answer;
}
//**********************************************************************************************

//*****************************  send Message  *************************************************
void server::sendMessage()
{
	
	unsigned int len;
	boost::system::error_code error;
	serverFile = fopen(/*path*/,"rb");
	if (serverFile != NULL)
	{
		loadsuccessMessage();
	}
	else
	{
		loadErrorMessage();
	}
	do
	{
		len = socketServer->write_some(boost::asio::buffer(outputMessage, strlen(outputMessage)), error);

	} while ((error.value() == WSAEWOULDBLOCK));
	if (error)
	{
		std::cout << "Hubo un error a la hora de enviar el mensaje del server al puerto" << error.message() << std::endl;
	}
}
//**********************************************************************************************

//*********************************  verifyMessage  ********************************************
bool server::verifyMessage()
{
	bool isEqual = false;
	for (int i = 0; (i < NUMBER_OF_PATHS) && (isEqual == false) ; i++)
	{
		isEqual = strcmp(path[i], inputMessage);
		if (isEqual == 0)
		{
			isEqual = true;
		}
	}
	return isEqual;
}
//**********************************************************************************************


//*********************************  loadsuccesMessage  *****************************************
void server::loadsuccessMessage(void)
{
	time_t now = time(0);
	
	string first_line =		"HTTP/1.1 200 OK\n";
	string second_line =	"Date: " + string(ctime(&now)) + '\n';
	string third_line =		"Location:" + string(SERVER_IP) + (string)inputMessage + '\n';
	string fourth_line =	"Cache-Control: max-age=30" + '\n';
	string fith_line =		"Expires:" /*+ lo que calcuala caro*/ + '\n';
	string sixth_line =		"Content-Length:" + lenInputMessage + '\n';
	string seventh_line =	"Content-Type:" /*+ Funcion que copia contenido*/ + '\n';

	string finalMessage = first_line + second_line + third_line + fourth_line + fith_line + sixth_line + seventh_line;
	outputMessage = finalMessage.c_str();
}
//**********************************************************************************************


server::~server() // destroyer
{
	server_acceptor->close();
	socketServer->close();
	delete server_acceptor;
	delete socketServer;
	delete IO_handler;
}


