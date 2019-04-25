/*************************************
 *  input_box SDL2
 *  AUTHOR  :   alahem monsef
 *  EMAIL   :   m.alahem09@gmail.com
 *  VERSION :   __
 *  DATE    :   1-feb-2019
 *  
 *  
 *
 *************************************/

typedef struct rgba_t {
    int r;
    int g;
    int b;
    int a;
}rgba_t;

typedef struct text_line_t {
    SDL_Surface* sur;
    SDL_Texture* tex;
    SDL_Rect rec;
}text_line_t;

#include "input_box.h"
#include "chat_box.h"

void init_ui()
{
    init_input_box();
    init_chat_box();
}

void update_ui()
{
    update_input_box();
    update_chat_box();
}

void draw_ui()
{
    draw_input_box();
    draw_chat_box();
}

int free_ui()
{
    free_input_box();
    free_chat_box();
}