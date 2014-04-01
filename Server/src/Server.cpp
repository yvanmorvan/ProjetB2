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
    int ClientAddrLength, MaxCC = 50, CRequest, i, j;
    HANDLE hProcessThread;
    SOCKET NewConnection, ClientSocket, ConnectedClients[50] = {0};
    struct thread_param p;

    char recvbuf[512];
    int recvbuflen = 512;
    char *sendbuff = "RSPS";


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

    if (listen(ListeningSocket, SOMAXCONN) == SOCKET_ERROR){
        cerr << "Impossible d'ecouter sur ce port." << endl;
        closesocket(ListeningSocket);
        WSACleanup();
        return 0;
    }

    cout << "\nServeur demarre, a l'ecoute sur le port "<< port << endl;
    cout << "Adresse : "<< inet_ntoa(ServerAddr.sin_addr) << endl;

    this->running = true;
    ClientAddrLength = sizeof(ClientAddr);
    // Stock des ports en cours d'exécution
    fd_set readfds;
    int activity = 0;


    // Démarrage du serveur
    while(this->running){

        // Clearing
        FD_ZERO(&readfds);
        // Ajout du socket d'écoute
        FD_SET(ListeningSocket, &readfds);

        // Ajout des anciens clients non déconnectés
        for (i=0; i < MaxCC; i++){
            if (ConnectedClients[i] > 0){
                FD_SET(ConnectedClients[i], &readfds);
            }
        }

        // Ecoute infinie sur tous les sockets enregistrés
        // Attente d'une activité
        activity = select(0, &readfds, NULL, NULL, NULL);

        // Nouvelle activité
        if (FD_ISSET(ListeningSocket, &readfds)){

            if ((NewConnection = accept(ListeningSocket, (SOCKADDR *)&ClientAddr, &ClientAddrLength)) == INVALID_SOCKET){
                cerr << "Connexion non acceptee. Erreur : "<< WSAGetLastError() << endl;
                closesocket(ListeningSocket);
                WSACleanup();
                return 0;
            }

            p.server = this;
            p.socket = NewConnection;

            // Ajout du nouveau client à la liste des clients connectés.
            for (i=0; i < MaxCC; i++){
                if (ConnectedClients[i] == 0){
                    ConnectedClients[i] = NewConnection;
                    break;
                }
            }

            cout << "\nClient connecte ! IP : "<< inet_ntoa(ClientAddr.sin_addr) << ", port : "<< ntohs(ClientAddr.sin_port)
                 << ", connexion : "<< NewConnection << endl;
        }

        // Démarrage du thread pour chaque client connecté
        for (i=0; i < MaxCC; i++){

            ClientSocket = ConnectedClients[i];

            if (FD_ISSET(ClientSocket, &readfds)){

                // Récupération de la requête du client
                // Réinitialisation du buffer
                for (j=0; j<recvbuflen; j++){
                    recvbuf[j] = '\0';
                }
                CRequest = recv(ClientSocket, recvbuf, recvbuflen, 0);


                if (CRequest == SOCKET_ERROR){

                    if (WSAGetLastError() == WSAECONNRESET){
                        cerr << "La connexion " << ClientSocket << " s'est terminee de maniere impromptue." <<endl;
                    }else{
                        cerr << "Erreur de lecture de la requete : "<< WSAGetLastError() << endl;
                    }

                    closesocket(ClientSocket);
                    ConnectedClients[i] = 0;

                }else if (CRequest == 0){

                    cerr << "La connexion " << ClientSocket << " s'est deconnectee." <<endl;
                    closesocket(ClientSocket);
                    ConnectedClients[i] = 0;

                }else{
                    cout << "Donnees recues : " << recvbuf << endl;
                    cout << "Envoi de la reponse a la connexion " << ClientSocket << endl;
                    send(ClientSocket, sendbuff, (int)strlen(sendbuff), 0);
                }

            }
        }

    } // END While running

    closesocket(ListeningSocket);
    WSACleanup();
    return 1;
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


    int result=1;
    int sendresult;
    char recvbuf[512];
    int recvbuflen = 512;
    char *sendbuff = "END";

    // Ecoute du client
    do{
        // Reset du buffer
        memset(&recvbuf, 0, recvbuflen);
        result = recv(socket, recvbuf, recvbuflen, 0);

        if (result > 0){
            cout << "La connexion "<< socket <<" a envoye : " << recvbuf << endl;
            send(socket, sendbuff, (int)strlen(sendbuff), 0);
            cout << "On envoie : " << sendbuff << endl;
        }

        else if (result == 0){
            cout << "Connexion fermee : "<< socket << endl;
            break;
        }else{
            cerr << "Erreur de reception des donnees. Erreur : " << WSAGetLastError() << endl;
            break;
        }

    }while(result > 0);

    // Fermeture de la connexion
    result = shutdown(socket, SD_SEND);
    if (result == SOCKET_ERROR){
        cerr << "Erreur lors du shutdown de la connexion. Erreur : "<< WSAGetLastError() << endl;
        closesocket(socket);
        WSACleanup();
        return 0;
    }

    closesocket(socket);
    WSACleanup();
    return 1;

}














