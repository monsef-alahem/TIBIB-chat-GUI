# TIBIB-chat-GUI

chat app made with SDL2

## compile GUI client


gcc -c -mwindows *.c

gcc -o app.exe *.o  -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lpthread -lws2_32


## compile server


gcc -c -mwindows *.c

gcc -o app.exe *.o -lpthread -lws2_32


![demo](screenshot.png)
