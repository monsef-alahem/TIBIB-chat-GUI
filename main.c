/*************************************
 *  AUTHOR  :   alahem monsef
 *  EMAIL   :   m.alahem09@gmail.com
 *  VERSION :   __
 *  DATE    :   1-feb-2019
 *
 *************************************/


//utiliser sdl2
#include <SDL2/SDL.h>

//ouvrir des images a part bmp avec sdl2
#include <SDL2/SDL_image.h>

//lire sons et musiques avec sdl2
#include <SDL2/SDL_mixer.h>

//afficher du text avec sdl2
#include <SDL2/SDL_ttf.h>


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


//variables globales client

SOCKET client_socket;
pthread_t thread;
char log[100][150];

char ip[20] = "127.0.0.1";
unsigned int port = 5666;
char pseudo[25] = "";
int i = 0;
int ret = 0;
char buff_out[512];
char buff_in[512];


//variables globales SDL2
SDL_Window* window;
SDL_Renderer* renderer;
Mix_Chunk *wave;
Mix_Music *music;
SDL_Event event;
#define KEYS 27
typedef struct key_t{
    char letter;
    SDL_Keycode code; 
}key_t;

#include "ui.h"
#include "client.h"

key_t key_table[KEYS] ={
    {'a',SDLK_a},
    {'b',SDLK_b},
    {'c',SDLK_c},
    {'d',SDLK_d},
    {'e',SDLK_e},
    {'f',SDLK_f},
    {'g',SDLK_g},
    {'h',SDLK_h},
    {'i',SDLK_i},
    {'j',SDLK_j},
    {'k',SDLK_k},
    {'l',SDLK_l},
    {'m',SDLK_m},
    {'n',SDLK_n},
    {'o',SDLK_o},
    {'p',SDLK_p},
    {'q',SDLK_q},
    {'r',SDLK_r},
    {'s',SDLK_s},
    {'t',SDLK_t},
    {'u',SDLK_u},
    {'v',SDLK_v},
    {'w',SDLK_w},
    {'x',SDLK_x},
    {'y',SDLK_y},
    {'z',SDLK_z},
    {' ',SDLK_SPACE}

};



int main(int argc, char**args)
{
    //demmarer sdl2
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);

    //initialiser l'affichage du texte et choix police
    TTF_Init();

    //initiliser audio
    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 );

    //contruire une fenêtre
    window = SDL_CreateWindow( "SDL2 template", 500, 300, 640, 480, 0 );
    //flag = SDL_WINDOW_FULLSCREEN for full screen
    Uint32 flag = 0;
    SDL_SetWindowFullscreen(window, flag);

    //création de l'ecran de dessin dans la fenêtre
    renderer = SDL_CreateRenderer( window, -1, 0 );
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

    //ouvrir un fichier son et musique
    wave = Mix_LoadWAV("CSI_Open.wav");
    music = Mix_LoadMUS("crimson.ogg");
    //music = Mix_LoadMUS("under_moon.ogg");

    //jouer son et musique
    //Mix_PlayMusic( music, -1);
    Mix_PlayChannel(-1, wave, 0);

    //création du dialogue auto-tapant
    init_ui();
    
    pthread_t thread;
    pthread_create(&thread, NULL, init_socket, NULL);

    int loop = 1;
    int i = 0;

    while ( loop )
    {
        //efface l'ecran de dessin avec le noir (r,v,b,a) a = alpha la transparence
        SDL_SetRenderDrawColor( renderer,0,0,0,255);
        SDL_RenderClear( renderer );

        //entrée de clavier
        while ( SDL_PollEvent( &event ) ) {
            if ( event.type == SDL_QUIT ) {                
                    loop = 0;
            } else if ( event.type == SDL_KEYDOWN ) {
                for (i = 0 ; i < KEYS ; i++) {
                    if (event.key.keysym.sym == key_table[i].code) {
                        static int once = 0;
                        if (!once) {
                            input_box_clean_lines();
                            once = 1;
                        }
                        input_box_append_letter(key_table[i].letter);
 /*                     input_box.text[idx] = key_table[i].letter; 
                        //strcat(dial_box[0]->text, key[i].letter); 
                        idx++;*/
                    }
                }

                if (event.key.keysym.sym == SDLK_RETURN && input_box.text[0][0] != 0) {
                    static int once = 0;
                    if (!once)
                    {
                        strcpy(pseudo, &input_box.text[0][0]);
                        strcpy(buff_out, pseudo);
                        ret = send(client_socket, buff_out, sizeof(buff_out), 0);
                        input_box_clean_lines();
                        once = 1;
                    } else { 
                        strcpy(buff_out, pseudo);
                        strcat(buff_out, " : ");
                        strcat(buff_out, input_box.text[0]);
                        int i;
                        for (i=1 ; i<input_box.line_pos ; i++)
                            strcat(buff_out, input_box.text[i]);
                        ret = send(client_socket, buff_out, sizeof(buff_out), 0);
                        input_box_clean_lines();
                    }
                }

                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    input_box.cursor_pos--;
                    input_box.text[input_box.line_pos][input_box.cursor_pos] = 0;
                }

                if (event.key.keysym.sym == SDLK_DELETE) {
                    input_box_clean_lines();
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    if (input_box.line_pos < 2){        
                        input_box.cursor_pos = input_box.line_pos*50;
                        input_box.line_pos++;
                    }
                }
            }
        }

        //ajoute des mots dans chaque boucle
        update_ui();
        //redessine la boite de dialogue
        draw_ui();

        //affiche l'ecran de dessin sur la fenetre
        SDL_RenderPresent(renderer);

        //pause de 16 millisecondes avant de recommencer la boucle "while"
        SDL_Delay(16);
    }

    //quand on quitte le programme (la boucle while) on detruit ce qu'on a crée
    //pour éviter les fuites de mémoire sur l'ordinateur
    Mix_FreeChunk(wave);
    Mix_FreeMusic(music);
    free_ui();
    free_socket();
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

}

