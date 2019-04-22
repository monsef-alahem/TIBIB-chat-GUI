/*************************************
 *  taped dialog box
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

typedef struct text_box_t {
    rgba_t col;
    rgba_t brd_col;
    int brd_tick;
    SDL_Rect rec;
    char text[3][50];
    text_line_t line[3];
    SDL_Rect txt_cursor;
    int cursor_pos;
    int line_pos;
    TTF_Font* font;
    int font_size;
}text_box_t;


//place to declare struct

text_box_t chat_box = {
    {0,255,240,255},
    {0,255,100,255},
    10,
    {20,340,600,120},
    {"","",""},
    {
    NULL,NULL,{30,350,370,190},
    NULL,NULL,{30,350+30,370,190},
    NULL,NULL,{30,350+60,370,190}
    },
    {8,20,10,20},
    0,
    0,
    NULL,
    20
};

//utilities
int chat_box_clean_lines()
{
    int i = 0;
    chat_box.line_pos = 0;
    chat_box.cursor_pos = 0;
    for (i=0;i<3;i++)
        memset(chat_box.text[i],0,50);
}

int chat_box_append_letter(char c)
{
    if (chat_box.line_pos < 2) {
        chat_box.text[chat_box.line_pos][chat_box.cursor_pos] = c;
        chat_box.cursor_pos++;
        if (chat_box.cursor_pos == 49) {
            chat_box.line_pos++;
            chat_box.cursor_pos = 0;
            return;
        }
    }
    if (chat_box.line_pos == 2) {
        chat_box.text[chat_box.line_pos][chat_box.cursor_pos] = c;
        if (chat_box.cursor_pos < 49)
            chat_box.cursor_pos++;
    }
}

void init_chat_box()
{
    chat_box.font = TTF_OpenFont("arial.ttf", chat_box.font_size);
    int i = 0;
    for (i=0;i<3;i++) {
        SDL_Color text_color = {0, 255, 240};
        //chat_box.line[i].sur = TTF_RenderText_Solid(font, chat_box.text[i], text_color);
        //chat_box.line[i].tex = SDL_CreateTextureFromSurface(renderer, chat_box.line[i].sur);
    }
}

void init_ui()
{
    init_chat_box();
}

void update_chat_box()
{
    //update text
/*    static int prev_cur_pos = 0;  
    if (prev_cur_pos != chat_box.cursor_pos) {
        chat_box.line_pos = chat_box.cursor_pos/50;
        prev_cur_pos = chat_box.cursor_pos;
    }*/
    
    //update graphics
    int i,w,h = 0;

    for (i=0;i<3;i++) {
        SDL_Color text_color = {0, 255, 240};
        chat_box.line[i].sur = TTF_RenderText_Solid(chat_box.font, chat_box.text[i], text_color);
        chat_box.line[i].tex = SDL_CreateTextureFromSurface(renderer, chat_box.line[i].sur);
        SDL_QueryTexture(chat_box.line[i].tex, NULL, NULL, &w, &h);
        chat_box.line[i].rec.w = w;
        chat_box.line[i].rec.h = h;
    }

    //update cursor position
    chat_box.txt_cursor.x = 10 + 30 + w;
    chat_box.txt_cursor.y = 350 + chat_box.line_pos*30;

    //correction of cursor pos bug
    if (chat_box.cursor_pos == 0)
        chat_box.txt_cursor.x = 10 + 30 + 0;

}


void update_ui()
{
    update_chat_box();
}

void draw_chat_box()
{
    //draw border
    SDL_Rect border = {
        20-chat_box.brd_tick/2,
        340-chat_box.brd_tick/2,
        600+chat_box.brd_tick,
        120+chat_box.brd_tick};
    SDL_SetRenderDrawColor( renderer,0,255,100,255); 
    SDL_RenderFillRect( renderer, &border);
    
    //draw chat box background rectangle
    SDL_SetRenderDrawColor( renderer,0,0,0,255); 
    SDL_RenderFillRect( renderer, &chat_box.rec);

    //draw text
    SDL_SetRenderDrawColor( renderer,
        0,255,100,255
    ); 
    int i;
    for (i = 0;i<3;i++) {     
        //SDL_RenderFillRect( renderer, &dial_box[i]->r);
        SDL_RenderCopy(renderer, chat_box.line[i].tex, NULL, (&chat_box.line[i].rec));
    }

    if (SDL_GetTicks()/500 % 2 == 0) {
        SDL_SetRenderDrawColor( renderer,0,255,100,255); 
        SDL_RenderFillRect( renderer, &chat_box.txt_cursor);
    }

}


void draw_ui()
{
    draw_chat_box();
}

int free_ui()
{
    int i;
    for (i = 0;i < 3;i++) { 
        SDL_FreeSurface(chat_box.line[i].sur);
        SDL_DestroyTexture(chat_box.line[i].tex);
    }
    TTF_CloseFont(chat_box.font);
}