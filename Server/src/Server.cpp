#include <iostream>
#include "../include/Server.h"
#include "../include/winsock2.h"

#pragma comment(lib, "../lib/libws2_32.a")

using namespace std;

Server::Server(int p){
    this->port = p;
    this->running = false;
}


int Server::init(){

    struct in_addr MyAddress;
    struct hostent *host;
    char HostName[100];
    WSADATA wsaData;

    // Initialisation WSA (version 2)
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        cerr << "Initialisation WSA échouee." << endl;
        return 0;
    }

    if (gethostname(HostName, 100) == SOCKET_ERROR){
        cerr << "Erreur : " << WSAGetLastError() << endl;
    }

    if ((host = gethostbyname(HostName)) == NULL){
        cerr << "Erreur : " << WSAGetLastError() << endl;
    }

    memcpy(&MyAddress.s_addr, host->h_addr_list[0], sizeof(MyAddress.s_addr));

    ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons( port );
	ServerAddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );

	cout <<"Serveur initialise !" << endl;

	return 0;

}


int Server::start(){

    SOCKADDR_IN ClientAddr;
    int ClientAddrLength;
    HANDLE hProcessThread;
    SOCKET NewConnection;
    struct thread_param p;

    if ((ListeningSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET){
        cerr << "Impossible d'ouvrir le socket : "<< WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }

    if (bind(ListeningSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR){
        cerr << "Impossible d'utiliser ce socket. Verifiez qu'aucun programme ne l'utilise deja." << endl;
        closesocket(ListeningSocket);
        WSACleanup();
        return 0;
    }

    if (listen(ListeningSocket, 0) == SOCKET_ERROR){
        cerr << "Impossible d'ecouter sur ce port." << endl;
        closesocket(ListeningSocket);
        WSACleanup();
        return 0;
    }

    cout << "\nServeur demarre, a l'ecoute sur le port "<< port << endl;
    cout << "Adresse : "<< inet_ntoa(ServerAddr.sin_addr) << endl;

    this->running = true;
    ClientAddrLength = sizeof(ClientAddr);

    // Démarrage du serveur
    while(this->running){

        if ((NewConnection = accept(ListeningSocket, (SOCKADDR *)&ClientAddr, &ClientAddrLength)) == INVALID_SOCKET){
            cerr << "Connexion non acceptee. Erreur : "<< WSAGetLastError() << endl;
            closesocket(ListeningSocket);
            WSACleanup();
            return 0;
        }

        p.server = this;
        p.socket = NewConnection;

        cout << "\nClient connecte ! IP : "<< inet_ntoa(ClientAddr.sin_addr) << ", port : "<< ntohs(ClientAddr.sin_port)<< endl;

        hProcessThread = CreateThread(NULL, 0, &Server::ThreadLauncher, &p, 0, NULL);

        if (hProcessThread == NULL){
            cerr << "Impossible de creer le processus."<< endl;
        }
    }

}


int Server::pause(){

    this->running = false;
    cout << "Serveur en pause."<< endl;
    closesocket(this->ListeningSocket);
    return 1;
}


/* IMPLEMENTATION DE LA GESTION DES THREADS */


DWORD Server::ClientThread(SOCKET socket){

    cout << "Thread ( "<<socket<<" ) a l'ecoute !\n" << endl;

    return 1;
}














