/*************************************
 *  Chat_client
 *  AUTHOR  :   alahem monsef
 *  EMAIL   :   m.alahem09@gmail.com
 *  VERSION :   2.0
 *  DATE    :   22-04-2019
 *************************************/
#define CHAT_LINE_LEN 150
#define CHAT_LINES 10
typedef struct chat_box_t {
    rgba_t col;
    rgba_t brd_col;
    int brd_tick;
    SDL_Rect rec;
    char text[CHAT_LINES][CHAT_LINE_LEN];
    text_line_t line[CHAT_LINES];
    int cursor_pos;
    int line_pos;
    TTF_Font* font;
    int font_size;
} chat_box_t;


//place to declare struct

chat_box_t chat_box = {
    {0,255,240,255},
    {0,255,100,255},
    10,
    {20,100,600,220},
    {"","",""},
    {NULL},
    0,
    0,
    NULL,
    17
};

//utilities
int chat_box_clean_lines()
{
    int i = 0;
    chat_box.line_pos = 0;
    chat_box.cursor_pos = 0;
    for (i=0;i<CHAT_LINES;i++)
        memset(chat_box.text[i],0,150);
}

void init_chat_box()
{
    chat_box.font = TTF_OpenFont("arial.ttf", chat_box.font_size);
    int i = 0;
    for (i=0;i<CHAT_LINES;i++) {
        SDL_Color text_color = {0, 255, 240};
        chat_box.line[i].rec = (SDL_Rect){chat_box.rec.x+10,(chat_box.rec.y)+(i * 20),chat_box.rec.w,chat_box.rec.h};

    }
}


void update_chat_box()
{
    //update graphics
    int i,w,h = 0;

    for (i=0;i<CHAT_LINES;i++) {
        SDL_Color text_color = {0, 255, 240};
        chat_box.line[i].sur = TTF_RenderText_Solid(chat_box.font, chat_box.text[i], text_color);
        chat_box.line[i].tex = SDL_CreateTextureFromSurface(renderer, chat_box.line[i].sur);
        SDL_QueryTexture(chat_box.line[i].tex, NULL, NULL, &w, &h);
        chat_box.line[i].rec.w = w;
        chat_box.line[i].rec.h = h;
    }

}

void draw_chat_box()
{
    //draw border
    SDL_Rect border = {
        chat_box.rec.x-chat_box.brd_tick/2,
        chat_box.rec.y-chat_box.brd_tick/2,
        chat_box.rec.w+chat_box.brd_tick,
        chat_box.rec.h+chat_box.brd_tick};
    SDL_SetRenderDrawColor( renderer,0,255,100,255); 
    SDL_RenderFillRect( renderer, &border);
    
    //draw input box background rectangle
    SDL_SetRenderDrawColor( renderer,0,0,0,255); 
    SDL_RenderFillRect( renderer, &chat_box.rec);

    //draw text
    SDL_SetRenderDrawColor( renderer,
        0,255,100,255
    ); 
    int i;
    for (i = 0;i<CHAT_LINES;i++) {     
        //SDL_RenderFillRect( renderer, &dial_box[i]->r);
        SDL_RenderCopy(renderer, chat_box.line[i].tex, NULL, (&chat_box.line[i].rec));
    }

}

void free_chat_box()
{
    int i;
    for (i = 0;i < CHAT_LINES;i++) { 
        SDL_FreeSurface(chat_box.line[i].sur);
        SDL_DestroyTexture(chat_box.line[i].tex);
    }
    TTF_CloseFont(chat_box.font);
}