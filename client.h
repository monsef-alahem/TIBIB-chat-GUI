/*************************************
 *  Chat_client
 *  AUTHOR  :   alahem monsef
 *  EMAIL   :   m.alahem09@gmail.com
 *  VERSION :   2.0
 *  DATE    :   22-04-2019
 *************************************/


void *reception(void *donnee)
{
    int ret;
    int i;
    int i_log = 0;
    int i_chat = 0;
    do {    
        ret = recv(client_socket, buff_in, sizeof(buff_in),0);
        strcpy(log[i_log], buff_in);
        if (i_chat<CHAT_LINES) {           
            strcpy(chat_box.text[i_chat], log[i_chat]);
            i_chat++;
        } else {
            for (i=0 ; i<CHAT_LINES ; i++)
                strcpy(chat_box.text[i], log[i_log-(CHAT_LINES-1-i)]);
        }
        i_log++;
    } while(ret != -1);

    //return &donnee;
}

void *init_socket(void * p)
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
        if(!pHostInfo){
            return 0;
        }
        //copier l'address dans pHostInfo
        memset(&nHostAddress, 0, sizeof(nHostAddress));
        memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length);
    } else {
    }

    //on creer le socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

/*}

conect_socket()
{*/
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

    do {
        ret = connect(client_socket, (SOCKADDR *)&Sin, sizeof Sin);
    } while (ret == -1);

/*    strcpy(buff_out, "user est connecter");
    send(client_socket, buff_out, sizeof(buff_out), 0);*/
    pthread_create(&thread, NULL, reception, NULL);
}
free_socket()
{
    closesocket(client_socket);

#ifdef WIN32
    WSACleanup();
#endif
    return EXIT_SUCCESS;
}