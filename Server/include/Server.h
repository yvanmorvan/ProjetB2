#ifndef SERVER_H
#define SERVER_H

#include "winsock2.h"
#pragma comment(../lib/libws2_32.a)


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
            return s->ClientThread(Thread->socket);

		}

    private:

        int           port;
		SOCKET	      ListeningSocket;
		bool          running;
		SOCKADDR_IN   ServerAddr;
		DWORD         ClientThread(SOCKET);
};

#endif // SERVER_H
