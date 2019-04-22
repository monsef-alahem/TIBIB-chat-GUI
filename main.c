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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

    //variables globales
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

#include "ui.h"


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
            }

            /* Add new text onto the end of our text */
            //strcat(text, event.text.text);
            //break;
            else if ( event.type == SDL_KEYDOWN ) {
                for (i = 0 ; i < KEYS ; i++) {
                    if (event.key.keysym.sym == key_table[i].code) {
                        chat_box_append_letter(key_table[i].letter);
 /*                       chat_box.text[idx] = key_table[i].letter; 
                        //strcat(dial_box[0]->text, key[i].letter); 
                        idx++;*/
                    }
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    chat_box_clean_lines();
                }
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    chat_box.cursor_pos--;
                    chat_box.text[chat_box.line_pos][chat_box.cursor_pos] = 0;
                }
                if (event.key.keysym.sym == SDLK_DELETE) {
                    chat_box_clean_lines();
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    if (chat_box.line_pos < 2){        
                        chat_box.cursor_pos = chat_box.line_pos*50;
                        chat_box.line_pos++;
                    }
                }
            }
        }

        //ajoute des mots dans chaque boucle
        update_ui();
        //redessine la boite de dialogue
        draw_ui();

        //affiche l'ecran de dessin sur la fenetre
        SDL_RenderPresent( renderer);

        //pause de 16 millisecondes avant de recommencer la boucle "while"
        SDL_Delay( 16 );
    }

    //quand on quitte le programme (la boucle while) on detruit ce qu'on a crée
    //pour éviter les fuites de mémoire sur l'ordinateur
    Mix_FreeChunk(wave);
    Mix_FreeMusic(music);
    free_ui();
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

}

