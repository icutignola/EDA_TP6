#include <iostream>
#include <cstdio>
#include <cstring>
#include <ctime>
#include "server.h"


using namespace std;

int main(void)
{
	server serverObj;
	cout << endl << "El servidor esta intentando conectarse con el cliente..." << PORT_CONECT << endl; //Empiezo a escuchas el puerto
	serverObj.startConnection(); 
	cout << "El servidor se puedo conectar al puerto" << PORT_CONECT << endl;
	if (serverObj.recivedMessage())
	{
		if (serverObj.verifyMessage())
		{
			cout << "Presione la tecla 'entrer' para enviar la respuesta" << endl;
			getchar();
			serverObj.sendMessage();
		}
	}
	
	Sleep(50); // Le damos 50ms para que llegue el mensaje antes de cerrar el socket.

	return 0;
}