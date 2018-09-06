#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>
#include "client.h"

using namespace std;

client::client(char * arg)
{
	host_arg = arg;
	IO_handler = new boost::asio::io_service();
	socket_forClient = new boost::asio::ip::tcp::socket(*IO_handler);
	client_resolver = new boost::asio::ip::tcp::resolver(*IO_handler);
}

client::~client() // destructor
{
	socket_forClient->close();
	delete client_resolver;
	delete socket_forClient;
	delete IO_handler;
}

void client::startConnection(const char* host) // cuando termino esto, los dos programas estan conectados
{
	endpoint = client_resolver->resolve(boost::asio::ip::tcp::resolver::query(host, HELLO_PORT_STR)); // en resolver pongo donde me quiero conectar y que puerto, se encargo de juntar en endpoint. hello_portt_str es el puerto del servidor
	boost::system::error_code error;
	boost::asio::connect(*socket_forClient, endpoint, error); // el resultado ponelo en socket for client
	if (error)
	{
		cout << "Error connecting to " << host << " Error Message: " << error.message() << endl;
		if (error.value() == boost::asio::error::connection_refused)
			cout << "Host " << host << " is not listening on the other side" << endl;
	}
	socket_forClient->non_blocking(true);
}

void client::receiveMessage()
{
	boost::system::error_code error;
	char buf[512]; // para poner la respuesta
	size_t len = 0;
	cout << "Receiving Message" << std::endl;
	boost::timer::cpu_timer t;//  creo un objeto tipo cpu timer que se llama t
	t.start();// lo inicializo (que empiece a contar)
	boost::timer::cpu_times pastTime = t.elapsed();// guarda el tiempo que paso
	double elapsedSeconds = 0.0;// 

	do
	{
		len = socket_forClient->read_some(boost::asio::buffer(buf), error); // leo de socket for client (osea de la coneccion con el server), lo meto en buf y si me da error, digo error. en len pone la cantidad leida

		boost::timer::cpu_times currentTime = t.elapsed();	//dijo diego que no va, creo

		if ((currentTime.wall - pastTime.wall) > 1e9)// 1 x 10^9
		{
			elapsedSeconds += (currentTime.wall - pastTime.wall) / 1e9;
			pastTime = currentTime; // pasaron x segundos
			cout << "Pasaron " << (int)floor(elapsedSeconds) << " segundos." << endl; // mientras no se termine imprimo que pasaron x segundos
		}

		if (!error)// si no tuve error, en lo que lei le puse un \0. 
			buf[len] = '\0'; //en el buffer donde guardo la cantidad leida, pongo el \0 para hacer un cout

	} while (error.value() == WSAEWOULDBLOCK);// se acaba cuando hay un error de read some (lee del socket que tenia con el server),  o cuando read some termina

	if (!error)
		cout << std::endl << "Server sais: " << buf << std::endl;
	else
		cout << "Error while trying to connect to server " << error.message() << std::endl;
}

void client::addPathToMessage(char * path)
{
	/*GET /path/filename HTTP/1.1 CRLF
	Host:127.0.0.1 CRLF
	CRLF*/

	string get = "GET";
	string pathToString = string(path);
	string iP = SERVER_IP;
	string message = get + ' ' + pathToString; // concateno el get con el path (entre get y path hay un espacio)
	string lastPartOfMessage = "HTTP/1.1 CRLF '\n' Host:" + iP + "CRLF '\n' CRLF";
	message = message + lastPartOfMessage;
	finalMessage = message.c_str();

}

const char * client::getFinalMessage()
{
	return finalMessage;
}
