/*************************************
 *  Chat_client
 *  AUTHOR  :   alahem monsef
 *  EMAIL   :   m.alahem09@gmail.com
 *  VERSION :   -
 *  DATE    :   2017
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//utilisation des thread car fonction recv bloquante
#include <pthread.h>

//les headers special pour réseaux sous windows
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
//pour convertir le hostname en ip
#include <netdb.h> 

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)

#define KNRM  "\x1B[0m"     //7
#define KRED  "\x1B[31m"    //12
#define KGRN  "\x1B[32m"    //10
#define KYEL  "\x1B[33m"    //14
#define KBLU  "\x1B[34m"    //9
#define KMAG  "\x1B[35m"    //13
#define KCYN  "\x1B[36m"    //11
#define KWHT  "\x1B[37m"    //15

//end color
#define NONE  "\033[0m"

//mettre d'accord windows et linux sur ces noms
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
#define PACKET_SIZE 100


#define TPSATTENTE 5


SOCKET client_socket;
pthread_t ThreadReception;
    char ip[20] = "127.0.0.1";
    unsigned int port = 5666;
    char pseudo[50] = "monsef";
    int i = 0;
    int ret = 0;
    char buff_out[512];
    char buff_in[512];

void *reception(void *donnee)
{
    do {    
    ret = recv(client_socket, buff_in, sizeof(buff_in),0);
    printf("%s", buff_in);
    } while(ret = -1);

    //return &donnee;
}



int main(void)
{

//demmarer socket sous windows
#ifdef WIN32
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);
#endif


    //variables pour capter et stocker l'adress ip 
    struct in_addr addr;
    struct hostent* pHostInfo;
    unsigned long nHostAddress;

    //si l'adresse est un nom de domaine, le convertir en address ip
    if (ip[0] < '0' || ip[0] > '9') {
        pHostInfo = gethostbyname(ip);
        addr.s_addr = *(u_long *) pHostInfo->h_addr_list[0];
        printf("its domain name !\n");
        if(!pHostInfo){
            printf("Could not resolve host name\n");
            return 0;
        }
        //copier l'address dans pHostInfo
        memset(&nHostAddress, 0, sizeof(nHostAddress));
        memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);
    } else {
        printf("its an ip adress !\n");
    }

    //on creer le socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
        printf("\nSocket client no %d ouvert\n",client_socket);

        //remplir la structure sin utilisé par le socket
        SOCKADDR_IN Sin;
        if (ip[0] < '0' || ip[0] > '9') {
            Sin.sin_addr.s_addr = inet_addr(inet_ntoa(addr));
        } else {
            Sin.sin_addr.s_addr = inet_addr(ip);
        }
        Sin.sin_family = AF_INET;
        Sin.sin_port = htons(port);
        //printf("Connection au %s sur le port %d en cours...\n",inet_addr(ip),port);
        printf("Connection au %s sur le port %d en cours...\n",ip,port);


        do {
            ret = connect(client_socket, (SOCKADDR *)&Sin, sizeof Sin);
        } while (ret == -1);

        printf("Connection au serveur effectue avec succes\n\n");


	

        //on envoie le pseudo au serveur
        send(client_socket, buff_out, sizeof(buff_out), 0);
        pthread_create(&ThreadReception, NULL, reception, NULL);

        do {

            //on recupere les saisies du clavier
            fgets(buff_out, sizeof(buff_out), stdin);
            //on les envoie au serveur
            send(client_socket, buff_out, sizeof(buff_out), 0);

        } while (1);


        printf("Connection avec le serveur interrompu\n");
        //on ferme le socket
        closesocket(client_socket);

#ifdef WIN32
    //on nettoie le wsa.
    WSACleanup();
#endif
    system("pause");
    return EXIT_SUCCESS;
}