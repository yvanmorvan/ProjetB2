#ifndef SERVER_H
#define SERVER_H

#include "winsock2.h"
#pragma comment(lib, "../lib/libws2_32.a")


class Server;


struct thread_param {
    Server* server;
    SOCKET socket;
};


class Server{

    public:

        Server(int);
		int init();
		int start ();
		int pause ();
		static DWORD WINAPI ThreadLauncher(void *p){

            // Methode statique
            struct thread_param *Thread = reinterpret_cast<struct thread_param*>(p);
            Server *s = Thread->server;
            // ClientThread se chargera d'écouter les évènements envoyés par le client.
            return s->ClientThread(Thread->socket);

		}

    private:

        int           port;
        bool          running;
		SOCKET	      ListeningSocket;
		SOCKADDR_IN   ServerAddr;
		DWORD         ClientThread(SOCKET);
};

#endif // SERVER_H
