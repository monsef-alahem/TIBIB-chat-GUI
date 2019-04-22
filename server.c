/*************************************
test client
 *************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//utilisation des thread car fonction recv bloquante
#include <pthread.h>

//les headers special pour réseaux pour windows
#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

//les headers special pour réseaux pour linux
#elif defined (linux)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//fonction close
#include <unistd.h>
//pour convertir le hostname en Ip
#include <netdb.h>

#define closesocket(s) close(s)

//abreviation des structures
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

//si ni linux ni windows on lance une erreur
#else
#error not defined for this platform
#endif
///fin des headers


//taille du packet réseaux
#define PACK_SIZE 128

#define MAX_CLIENT 10


SOCKET server_socket;

typedef struct Client {
    SOCKET client_socket;
    char buff[512];
    char pseudo[50];
	int is_active;
    pthread_t thread;
} Client;

static Client c[MAX_CLIENT];


SOCKADDR_IN Sin = {0};
unsigned short port = 5666;
int niv_securite = 0;



int init_server()
{
    int ret = 0;
    
    //init
    #ifdef WIN32
        WSADATA WSAData;
        //initaliser winsock
        WSAStartup(MAKEWORD(2,0), &WSAData);
    #endif


    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    //on rempli la structure
    Sin.sin_addr.s_addr    = htonl(INADDR_ANY);
    Sin.sin_family    = AF_INET;
    Sin.sin_port    = htons(port);
    ret = bind(server_socket, (SOCKADDR *)&Sin, sizeof(Sin));

    ret = listen(server_socket, 2);
    printf("Le Serveur ecoute sur le port %d\nEn attente de la connections des clients\n",port);

}


 void *find_client(void* data)
{
    int id = (int) data;
    c[id].is_active = 1;
    int ret = 0;
    printf("int : %d\n", id);
    SOCKADDR_IN CSin = {0};
    int sizeofcsin = sizeof(CSin);
    int i = 0;
    do {
        c[id].client_socket = accept(server_socket, (SOCKADDR *)&CSin, &sizeofcsin);
        Sleep(500);
    } while(c[id].client_socket == -1);
    //return NULL;
//}


//static void *talk_to_client(int id)
//{

    memset(c[id].buff, 0, sizeof(c[id].buff));
    recv(c[id].client_socket, c[id].pseudo, sizeof(c[id].pseudo), 0);
    printf("%s\n", c[id].pseudo);
    do {
        recv(c[id].client_socket, c[id].buff, sizeof(c[id].buff), 0);
        ret = send(c[id].client_socket, c[id].buff, sizeof(c[id].buff), 0);
        printf("%s", c[id].buff);
    } while(ret != -1);

    if(c[id].client_socket == -1 || ret == -1) {
    	shutdown(c[id].client_socket, 2);
        closesocket(c[id].client_socket);
        c[id].is_active = 0;
    }
    return NULL;
}

int run_server(int id)
{

    while(1){
        for (id = 0 ; id < MAX_CLIENT ; id++)
            if(!c[id].is_active)
                pthread_create(&c[id].thread, NULL, find_client, (void*)id);
        Sleep(500);
    }
    //talk_to_client(id);
}

int shutdown_server()
{
    shutdown(server_socket, 2);
    closesocket(server_socket);
    #ifdef WIN32
        WSACleanup();
    #endif
    printf("shutdown server is success\n");
}

int main(void)
{
    int id = 0;
    init_server();
    run_server(id);
    shutdown_server();
    system("pause");
}

/*
init server socket
bind_to_the port
listen_in_the_port
accept_client
receve_from_client


/*************************************
test client
 *************************************/
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//utilisation des thread car fonction recv bloquante
#include <pthread.h>

//les headers special pour réseaux pour windows
#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

//les headers special pour réseaux pour linux
#elif defined (linux)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//fonction close
#include <unistd.h>
//pour convertir le hostname en Ip
#include <netdb.h>


//abreviation des structures
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

//si ni linux ni windows on lance une erreur
#else
#error not defined for this platform
#endif
///fin des headers


//taille du packet réseaux
#define PACK_SIZE 128

#define MAX_CLIENT 10


SOCKET server_socket;

typedef struct Client {
    SOCKET client_socket;
    char buff[512];
    char pseudo[50];
    pthread_t thread;
    pthread_t thread2;
} Client;

Client c[10];


SOCKADDR_IN Sin = {0};

SOCKADDR_IN CSin = {0};
int sizeofcsin = sizeof(CSin);

unsigned short port = 5666;
int niv_securite = 0;



int init_server()
{
    int ret = 0;
    
    //init
    #ifdef WIN32
        WSADATA WSAData;
        //initaliser winsock
        WSAStartup(MAKEWORD(2,0), &WSAData);
    #endif


    //structure qui va contenir les informations  de notre socket serveur

    //creation du socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    //on rempli la structure
    Sin.sin_addr.s_addr    = htonl(INADDR_ANY);
    Sin.sin_family    = AF_INET;
    Sin.sin_port    = htons(port);
    ret = bind(server_socket, (SOCKADDR *)&Sin, sizeof(Sin));
}

void *init_find_client(int id)
{
    int ret = 0;
    printf("int : %d\n", id);
    ret = listen(server_socket, 2);
    printf("Le Serveur ecoute sur le port %d\nEn attente de la connections des clients\n",port);
}

void *run_find_client(int id)
{
    if(c[id].client_socket != -1) {
        return;
    }
    c[id].client_socket = accept(server_socket, (SOCKADDR *)&CSin, &sizeofcsin);
}

void *find_client(int id)
{
    init_find_client(id);
    pthread_create(&c[id].thread,NULL,run_find_client(id),NULL);
}


void *init_talk_to_client(int id)
{
    memset(c[id].buff, 0, sizeof(c[id].buff));
    recv(c[id].client_socket, c[id].pseudo, sizeof(c[id].pseudo), 0);
    printf("%s\n", c[id].pseudo);
}

void *run_talk_to_client(int id)
{
    //memset(buff, 0, sizeof(buff));
    int ret = 0;
    recv(c[id].client_socket, c[id].buff, sizeof(c[id].buff), 0);
    ret = send(c[id].client_socket, c[id].buff, sizeof(c[id].buff), 0);
    printf("%s", c[id].buff);
}

void *talk_to_client(int id)
{
    init_talk_to_client(id);
    pthread_create(&c[id].thread2,NULL,run_talk_to_client(id),NULL);
}

int run_server(int id)
{
    while(1){
        find_client(id);
    }
        talk_to_client(id);
}

int shutdown_server()
{
    #ifdef WIN32
        WSACleanup();
    #endif
    printf("shutdown server is success\n");
}

int main(void)
{
    int id = 0;
    init_server();
    run_server(id);
    shutdown_server();
}
*/
