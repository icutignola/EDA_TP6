#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "client.h"
#include "parse_path.h"

using namespace std;

int main(int argc, char* argv[])
{
	client conquering;
	cout << "Trying to connect to " << SERVER_IP << " on port " << HELLO_PORT_STR << std::endl;
	conquering.startConnection(SERVER_IP);
	conquering.receiveMessage();
	cout << "Press Enter to exit..." << std::endl;
	getchar();
	return 0;
}

/*
servidor escucha al puerto 80 (el servidor va a ser el mas largo de los dos)
una vez que tiene un socket valido, se conecta
recibe el comando CRLF (ojo que no hay terminador \0) tiene 28 caracteres, sin \0.
si recibo la linea GET/path/
HOST:...
CRLF
una vez que recibio eso, el path y el filename representan un directorio un archivo dentro de la carpeta donde esta corriendo el programa
de encontrarse, el server respondera:
http...
date
location
cache

putty -> allseesions output

en logging: log file name: eda2.log
host name ip adress: www.google.com
port: 80
// el putty lo usamos para debbugear, despues usamos el chrome

*** client

client.exe recibe:	localhost\cgibin\index.html
se conecta a local host y espera la respuesta del server
una vez conectado le enviara el pedido al servidor
cuando el server le responda guarda la respuesta del server en formato txt (un txt que tiene solo lo que respondio el server)

fopen ( "filename", "rb");
**importante ** short c; tiene que ser un short para diferenciar los valores validos del eof (diferencio el valor posible FF del EOF)

c= fgetc (fp);// un archivo pensado en binario puede tener un valor entre 0000  y 00FF y el caso particular del eof que es FFFF.
if (c != eof)
fputc (c);

*/