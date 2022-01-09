#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <string.h>
#define realwidth 1000
#define width 700
#define height 700
#include "AI.h"
#include "Create.h"
#include "MoveAndRedo.h"
int e=0,f=0,p,x=0,player=1,totalmoves=0,maxmoves=0,dim=0,computer,starttime,endtime,diftime = 0,cI=0,cO=0,cII=0;
char game=0,ss[1],sG='0';
struct{
    char name[13];
    int points;
    int moves;
}player1={"",0,0},player2={"Computer",0,0};
bool mouse = false, SDLrun = false,quit = false,ran = false;
FILE *saved;
SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *font;
SDL_Surface *imgwallpaper;
SDL_Surface *saveimg;
SDL_Surface *imgredo;
SDL_Surface *imgundo;
SDL_Surface *returnbuttonimg;
SDL_Surface *imagedots;
SDL_Texture *wallpaper;
SDL_Texture *redoicon;
SDL_Texture *undoicon;
SDL_Texture *dots;
SDL_Texture *saveicon;
SDL_Texture *returnbutton;
SDL_Surface *logoimg;
SDL_Texture *logo;
Mix_Music *backgroundmusic;
void initSDL(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);


    TTF_Init();
    IMG_Init(IMG_INIT_PNG | IMG_INIT_PNG);

    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);

    backgroundmusic = Mix_LoadMUS("RAF - Self Control - (1984).mp3");

    window = SDL_CreateWindow("dots and boxes",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,realwidth,height,SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    font = TTF_OpenFont("Lato-Italic.ttf",23);

    imgwallpaper = IMG_Load("wallpaper.jpg");
    saveimg = IMG_Load("saveicon.png");
    imgredo = IMG_Load("redo.png");
    imgundo = IMG_Load("undo.png");
    returnbuttonimg = IMG_Load("BACK.png");
    imagedots = IMG_Load("dots.png");
    logoimg = IMG_Load("LOGO.jpg");
    wallpaper = SDL_CreateTextureFromSurface(renderer,imgwallpaper);
    redoicon = SDL_CreateTextureFromSurface(renderer,imgredo);
    undoicon = SDL_CreateTextureFromSurface(renderer,imgundo);
    dots = SDL_CreateTextureFromSurface(renderer,imagedots);
    saveicon = SDL_CreateTextureFromSurface(renderer,saveimg);
    returnbutton = SDL_CreateTextureFromSurface(renderer,returnbuttonimg);
    logo = SDL_CreateTextureFromSurface(renderer,logoimg);
    Mix_PlayMusic(backgroundmusic,-1);

}

void killSDL(){
    SDL_DestroyTexture(logo);
    SDL_DestroyTexture(returnbutton);
    SDL_DestroyTexture(saveicon);
    SDL_DestroyTexture(dots);
    SDL_DestroyTexture(undoicon);
    SDL_DestroyTexture(redoicon);
    SDL_DestroyTexture(wallpaper);
    SDL_FreeSurface(logoimg);
    SDL_FreeSurface(imagedots);
    SDL_FreeSurface(returnbuttonimg);
    SDL_FreeSurface(imgundo);
    SDL_FreeSurface(imgredo);
    SDL_FreeSurface(saveimg);
    SDL_FreeSurface(imgwallpaper);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(backgroundmusic);
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}


void update(char world[dim][dim],int mx1,int my1){
    int shift=0,L=0,wz=0,hv=0,e=0,f=0,linex,liney;
    float a=0,b=0,c=0,d=0;
    switch(dim){
    case 7:
        a = 0.8;
        b = 0.8;
        c = 0.3;
        d = 0.3;
        e = 20;
        f = 20;
        shift = 30;
        L = 50;
        wz = 200;
        hv = 200;
        break;
    case 11:
        a = 0.65;
        b = 0.6;
        c = 0.38;
        d = 0.35;
        e = 15;
        f = 15;
        shift = 8;
        L = 50;
        wz = 130;
        hv = 125;
        break;
    case 15:
        a = 0.65;
        b = 0.65;
        c = 0.29;
        d = 0.24;
        e = 15;
        f = 15;
        shift = 8;
        L = 30;
        wz = 90;
        hv = 90;
        break;

    }
    char scoreline[30] = "";
    char scorenum[5] = "";
    strcat(scoreline,player1.name);
    strcat(scoreline," (1): ");
    itoa(player1.points,scorenum,10);
    strcat(scoreline,scorenum);

    SDL_Rect pos = {.x = 0,.y = 0,realwidth,height};

    SDL_RenderCopy(renderer,wallpaper,NULL,&pos);

    SDL_Color color = {200,200,200,255};

    SDL_Surface *scoretextimg1 = TTF_RenderText_Blended(font,scoreline,color);

    SDL_Texture *scoretext1 = SDL_CreateTextureFromSurface(renderer,scoretextimg1);

    SDL_FreeSurface(scoretextimg1);

    pos.x = 710;
    pos.y = 300;

    SDL_QueryTexture(scoretext1,NULL,NULL,&pos.w,&pos.h);

    SDL_RenderCopy(renderer,scoretext1,NULL,&pos);

    SDL_DestroyTexture(scoretext1);


    scoreline[0] = '\0';
    scorenum[0] = '\0';
    if(!computer){
        strcat(scoreline,player2.name);
        strcat(scoreline," (2): ");
        itoa(player2.points,scorenum,10);
        strcat(scoreline,scorenum);
    }else{
        strcat(scoreline,"Computer (2): ");
        itoa(player2.points,scorenum,10);
        strcat(scoreline,scorenum);
    }
    SDL_Surface *scoretextimg2 = TTF_RenderText_Blended(font,scoreline,color);

    SDL_Texture *scoretext2 = SDL_CreateTextureFromSurface(renderer,scoretextimg2);

    SDL_FreeSurface(scoretextimg2);

    pos.x = 710;
    pos.y = 320;

    SDL_QueryTexture(scoretext2,NULL,NULL,&pos.w,&pos.h);

    SDL_RenderCopy(renderer,scoretext2,NULL,&pos);

    SDL_DestroyTexture(scoretext2);

    scoreline[0] = '\0';
    scorenum[0] = '\0';
    strcat(scoreline,"PLAYER TURN : ");
    itoa(player,scorenum,10);
    strcat(scoreline,scorenum);

    SDL_Surface *playerturntextimg = TTF_RenderText_Blended(font,scoreline,color);

    SDL_Texture *playerturntext = SDL_CreateTextureFromSurface(renderer,playerturntextimg);

    SDL_FreeSurface(playerturntextimg);

    pos.x = 710;
    pos.y = 490;

    SDL_QueryTexture(playerturntext,NULL,NULL,&pos.w,&pos.h);

    SDL_RenderCopy(renderer,playerturntext,NULL,&pos);

    SDL_DestroyTexture(playerturntext);


    scoreline[0] = '\0';
    scorenum[0] = '\0';
    strcat(scoreline,"PLAYER ONE MOVES: ");
    itoa(player1.moves,scorenum,10);
    strcat(scoreline,scorenum);

    SDL_Surface *player1movesimg = TTF_RenderText_Blended(font,scoreline,color);

    SDL_Texture *player1moves = SDL_CreateTextureFromSurface(renderer,player1movesimg);

    SDL_FreeSurface(player1movesimg);

    pos.x = 710;
    pos.y = 350;

    SDL_QueryTexture(player1moves,NULL,NULL,&pos.w,&pos.h);

    SDL_RenderCopy(renderer,player1moves,NULL,&pos);

    SDL_DestroyTexture(player1moves);


    scoreline[0] = '\0';
    scorenum[0] = '\0';
    strcat(scoreline,"PLAYER TWO MOVES: ");
    itoa(player2.moves,scorenum,10);
    strcat(scoreline,scorenum);

    SDL_Surface *player2movesimg = TTF_RenderText_Blended(font,scoreline,color);

    SDL_Texture *player2moves = SDL_CreateTextureFromSurface(renderer,player2movesimg);

    SDL_FreeSurface(player2movesimg);

    pos.x = 710;
    pos.y = 380;

    SDL_QueryTexture(player2moves,NULL,NULL,&pos.w,&pos.h);

    SDL_RenderCopy(renderer,player2moves,NULL,&pos);

    SDL_DestroyTexture(player2moves);

    scoreline[0] = '\0';
    scorenum[0] = '\0';
    strcat(scoreline,"REMAINED MOVES: ");
    itoa((2 * (dim/2) * (dim/2 + 1)) - (player1.moves + player2.moves),scorenum,10);

    strcat(scoreline,scorenum);

    SDL_Surface *remainedmovesimg = TTF_RenderText_Blended(font,scoreline,color);

    SDL_Texture *remainedmove = SDL_CreateTextureFromSurface(renderer,remainedmovesimg);

    SDL_FreeSurface(remainedmovesimg);

    pos.x = 710;
    pos.y = 410;

    SDL_QueryTexture(remainedmove,NULL,NULL,&pos.w,&pos.h);

    SDL_RenderCopy(renderer,remainedmove,NULL,&pos);

    SDL_DestroyTexture(remainedmove);

    endtime = SDL_GetTicks();
    scoreline[0] = '\0';
    scorenum[0] = '\0';
    strcat(scoreline,"TIME: ");
    itoa(((((endtime - starttime)/1000 + diftime)/60)%60),scorenum,10);
    strcat(scoreline,scorenum);
    strcat(scoreline," : ");
    itoa(((((endtime - starttime)/1000)+diftime)%60),scorenum,10);

    strcat(scoreline,scorenum);

    SDL_Surface *timepassedimg = TTF_RenderText_Blended(font,scoreline,color);

    SDL_Texture *timepassed = SDL_CreateTextureFromSurface(renderer,timepassedimg);

    SDL_FreeSurface(timepassedimg);

    pos.x = 710;
    pos.y = 440;

    SDL_QueryTexture(timepassed,NULL,NULL,&pos.w,&pos.h);

    SDL_RenderCopy(renderer,timepassed,NULL,&pos);

    SDL_DestroyTexture(timepassed);

    if(x){
        strcpy(scoreline,"Game will be saved in ");
        strncat(scoreline,&sG,1);
        SDL_Surface *remainedmovesimg = TTF_RenderText_Blended(font,scoreline,color);
        SDL_Texture *remainedmove = SDL_CreateTextureFromSurface(renderer,remainedmovesimg);
        SDL_FreeSurface(remainedmovesimg);

        pos.x = 710;
        pos.y = 540;

        SDL_QueryTexture(remainedmove,NULL,NULL,&pos.w,&pos.h);

        SDL_RenderCopy(renderer,remainedmove,NULL,&pos);

        SDL_DestroyTexture(remainedmove);

    }

    pos.x = 710;
    pos.y = 120;
    pos.w = 100;
    pos.h = 100;
    SDL_RenderCopy(renderer,saveicon,NULL,&pos);


    pos.x = 900;
    pos.y = 20;
    pos.w = 80;
    pos.h = 80;
    SDL_RenderCopy(renderer,redoicon,NULL,&pos);



    pos.x = 720;
    pos.y = 20;
    pos.w = 80;
    pos.h = 80;
    SDL_RenderCopy(renderer,undoicon,NULL,&pos);

    SDL_Texture *returnbutton = SDL_CreateTextureFromSurface(renderer,returnbuttonimg);


    pos.x = 700;
    pos.y = 600;
    pos.w = 280;
    pos.h = 50;

    SDL_RenderCopy(renderer,returnbutton,NULL,&pos);

    SDL_DestroyTexture(returnbutton);


    if(mouse){
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_GetMouseState(&linex,&liney);
        SDL_RenderDrawLine(renderer,mx1,my1,linex,liney);
    }
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            if(world[i][j] == '0' || world[i][j] == '2' || world[i][j] == '3'){

                if((i%2 && !(j%2)) && world[i][j] != ' '){ // HORIZONTAL
                    pos.x = (i-a) * width/dim + shift;
                    pos.y = (j) * height/dim + shift;
                    pos.w = wz;
                    pos.h = L;
                    if(cO){
                        if(world[i][j] == '2')
                            SDL_SetRenderDrawColor(renderer,0,255,0,255);
                        else if(world[i][j] == '3')
                            SDL_SetRenderDrawColor(renderer,230,0,230,255);
                    }else{
                        if(world[i][j] == '2')
                            SDL_SetRenderDrawColor(renderer,255,150,0,255);
                        else if(world[i][j] == '3')
                            SDL_SetRenderDrawColor(renderer,0,200,255,255);}

                    SDL_RenderFillRect(renderer,&pos);
                }else if((!(i%2) && (j%2) )&& world[i][j] != ' '){ // VERTICAL
                    pos.x = (i) * width/dim + shift;
                    pos.y = (j-b) * height/dim + shift;
                    pos.w = L;
                    pos.h = hv;
                    if(cO){
                        if(world[i][j] == '2')
                            SDL_SetRenderDrawColor(renderer,0,255,0,255);
                        else if(world[i][j] == '3')
                            SDL_SetRenderDrawColor(renderer,230,0,230,255);
                    }else{
                        if(world[i][j] == '2')
                            SDL_SetRenderDrawColor(renderer,255,150,0,255);
                        else if(world[i][j] == '3')
                            SDL_SetRenderDrawColor(renderer,0,200,255,255);}
                    SDL_RenderFillRect(renderer,&pos);
                }

            }
            else if(world[i][j] == 'X'){

                if(cI){SDL_SetRenderDrawColor(renderer,0,0,0,255);}
                else{SDL_SetRenderDrawColor(renderer,255,0,0,255);}
                pos.x = (i-a+c) * width/dim + shift;
                pos.y = (j-b+d) * height/dim + shift;
                pos.w = wz - e;
                pos.h = hv - f;

                SDL_RenderFillRect(renderer,&pos);
            }
            else if(world[i][j] == 'O'){

                if(cI){SDL_SetRenderDrawColor(renderer,255,255,255,255);}
                else{SDL_SetRenderDrawColor(renderer,0,0,255,255);}
                pos.x = (i-a+c) * width/dim + shift;
                pos.y = (j-b+d) * height/dim + shift;
                pos.w = wz - e;
                pos.h = hv - f;

                SDL_RenderFillRect(renderer,&pos);
            }
        }
    }
    for(int i=0;i<dim;i=i+2){
        for(int j=0;j<dim;j=j+2){
            pos.x = i * width/dim + shift;
            pos.y = j * height/dim + shift;
            pos.w = L;
            pos.h = L;
            SDL_RenderCopy(renderer,dots,NULL,&pos);
        }
    }

    SDL_RenderPresent(renderer);

}

void updateSave(bool save){
    SDL_Rect pos;
    if(!save){
        SDL_Rect pos = {.x = 0,.y = 0,realwidth,height};
        SDL_RenderCopy(renderer,wallpaper,NULL,&pos);

        SDL_Surface *logoimg = IMG_Load("LOGO.jpg");
        SDL_Texture *logo = SDL_CreateTextureFromSurface(renderer,logoimg);
        SDL_FreeSurface(logoimg);
        pos.x = 250;
        pos.y = 0;
        pos.w = 500;
        pos.h = 300;
        SDL_RenderCopy(renderer,logo,NULL,&pos);
        SDL_DestroyTexture(logo);

        SDL_Surface *backiconimg = IMG_Load("Back.png");
        SDL_Texture *backicon = SDL_CreateTextureFromSurface(renderer,backiconimg);
        SDL_FreeSurface(backiconimg);
        pos.x = 510;
        pos.y = 600;
        pos.w = 300;
        pos.h = 80;
        SDL_RenderCopy(renderer,backicon,NULL,&pos);
        SDL_DestroyTexture(backicon);
    }
    SDL_Surface *save1iconimg = IMG_Load("1saveimg.png");
    SDL_Texture *save1icon = SDL_CreateTextureFromSurface(renderer,save1iconimg);
    SDL_FreeSurface(save1iconimg);
    if(save){
        pos.x = 800;
        pos.y = 120;
        pos.w = 80;
        pos.h = 50;
    }
    else{
        pos.x = 190;
        pos.y = 400;
        pos.w = 300;
        pos.h = 80;
    }

    SDL_RenderCopy(renderer,save1icon,NULL,&pos);
    SDL_DestroyTexture(save1icon);

    SDL_Surface *save2iconimg = IMG_Load("2saveimg.png");
    SDL_Texture *save2icon = SDL_CreateTextureFromSurface(renderer,save2iconimg);
    SDL_FreeSurface(save2iconimg);
    if(save){
        pos.x = 900;
        pos.y = 120;
        pos.w = 80;
        pos.h = 50;
    }else{
        pos.x = 510;
        pos.y = 400;
        pos.w = 300;
        pos.h = 80;
    }
    SDL_RenderCopy(renderer,save2icon,NULL,&pos);
    SDL_DestroyTexture(save2icon);

    SDL_Surface *save3iconimg = IMG_Load("3saveimg.png");
    SDL_Texture *save3icon = SDL_CreateTextureFromSurface(renderer,save3iconimg);
    SDL_FreeSurface(save3iconimg);
    if(save){
        pos.x = 800;
        pos.y = 180;
        pos.w = 80;
        pos.h = 50;
    }else{
        pos.x = 190;
        pos.y = 500;
        pos.w = 300;
        pos.h = 80;
    }
    SDL_RenderCopy(renderer,save3icon,NULL,&pos);
    SDL_DestroyTexture(save3icon);

    SDL_Surface *save4iconimg = IMG_Load("4saveimg.png");
    SDL_Texture *save4icon = SDL_CreateTextureFromSurface(renderer,save4iconimg);
    SDL_FreeSurface(save4iconimg);
    if(save){
        pos.x = 900;
        pos.y = 180;
        pos.w = 80;
        pos.h = 50;
    }else{
        pos.x = 510;
        pos.y = 500;
        pos.w = 300;
        pos.h = 80;
    }
    SDL_RenderCopy(renderer,save4icon,NULL,&pos);
    SDL_DestroyTexture(save4icon);

    SDL_Surface *save5iconimg = IMG_Load("5saveimg.png");
    SDL_Texture *save5icon = SDL_CreateTextureFromSurface(renderer,save5iconimg);
    SDL_FreeSurface(save5iconimg);
    if(save){
        pos.x = 800;
        pos.y = 240;
        pos.w = 80;
        pos.h = 50;
    }else{
        pos.x = 190;
        pos.y = 600;
        pos.w = 300;
        pos.h = 80;
    }
    SDL_RenderCopy(renderer,save5icon,NULL,&pos);
    SDL_DestroyTexture(save5icon);


    SDL_RenderPresent(renderer);
}
void gamemenu(){

    SDL_Rect pos = {.x = 0,.y = 0,realwidth,height};
    SDL_RenderCopy(renderer,wallpaper,NULL,&pos);

    SDL_Surface *logoimg = IMG_Load("LOGO.jpg");
    SDL_Texture *logo = SDL_CreateTextureFromSurface(renderer,logoimg);
    SDL_FreeSurface(logoimg);
    pos.x = 250;
    pos.y = 0;
    pos.w = 500;
    pos.h = 300;
    SDL_RenderCopy(renderer,logo,NULL,&pos);
    SDL_DestroyTexture(logo);

    SDL_Surface *newgameiconimg = IMG_Load("NEWGAME.png");
    SDL_Texture *newgameicon = SDL_CreateTextureFromSurface(renderer,newgameiconimg);
    SDL_FreeSurface(newgameiconimg);
    pos.x = 190;
    pos.y = 400;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,newgameicon,NULL,&pos);
    SDL_DestroyTexture(newgameicon);

    SDL_Surface *loadgameiconimg = IMG_Load("LOADGAME.png");
    SDL_Texture *loadgameicon = SDL_CreateTextureFromSurface(renderer,loadgameiconimg);
    SDL_FreeSurface(loadgameiconimg);
    pos.x = 510;
    pos.y = 400;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,loadgameicon,NULL,&pos);
    SDL_DestroyTexture(loadgameicon);

    SDL_Surface *leaderboardiconimg = IMG_Load("LEADERBOARD.png");
    SDL_Texture *leaderboardicon = SDL_CreateTextureFromSurface(renderer,leaderboardiconimg);
    SDL_FreeSurface(leaderboardiconimg);
    pos.x = 190;
    pos.y = 500;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,leaderboardicon,NULL,&pos);
    SDL_DestroyTexture(leaderboardicon);

    SDL_Surface *settingsiconimg = IMG_Load("settings.png");
    SDL_Texture *settingsicon = SDL_CreateTextureFromSurface(renderer,settingsiconimg);
    SDL_FreeSurface(settingsiconimg);
    pos.x = 510;
    pos.y = 500;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,settingsicon,NULL,&pos);
    SDL_DestroyTexture(settingsicon);

    SDL_Surface *exiticonimg = IMG_Load("exit.png");
    SDL_Texture *exiticon = SDL_CreateTextureFromSurface(renderer,exiticonimg);
    SDL_FreeSurface(exiticonimg);
    pos.x = 350;
    pos.y = 600;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,exiticon,NULL,&pos);
    SDL_DestroyTexture(exiticon);

    SDL_RenderPresent(renderer);
}

void newgamemenu(){
    SDL_Surface *imgwallpaper = IMG_Load("wallpaper.jpg");
    SDL_Texture *wallpaper = SDL_CreateTextureFromSurface(renderer,imgwallpaper);
    SDL_FreeSurface(imgwallpaper);
    SDL_Rect pos = {.x = 0,.y = 0,realwidth,height};
    SDL_RenderCopy(renderer,wallpaper,NULL,&pos);
    SDL_DestroyTexture(wallpaper);

    SDL_Surface *logoimg = IMG_Load("LOGO.jpg");
    SDL_Texture *logo = SDL_CreateTextureFromSurface(renderer,logoimg);
    SDL_FreeSurface(logoimg);
    pos.x = 250;
    pos.y = 0;
    pos.w = 500;
    pos.h = 300;
    SDL_RenderCopy(renderer,logo,NULL,&pos);
    SDL_DestroyTexture(logo);

    SDL_Surface *newgameiconimg = IMG_Load("vscomputer.png");
    SDL_Texture *newgameicon = SDL_CreateTextureFromSurface(renderer,newgameiconimg);
    SDL_FreeSurface(newgameiconimg);
    pos.x = 350;
    pos.y = 400;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,newgameicon,NULL,&pos);
    SDL_DestroyTexture(newgameicon);

    SDL_Surface *loadgameiconimg = IMG_Load("vsplayer.png");
    SDL_Texture *loadgameicon = SDL_CreateTextureFromSurface(renderer,loadgameiconimg);
    SDL_FreeSurface(loadgameiconimg);
    pos.x = 350;
    pos.y = 500;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,loadgameicon,NULL,&pos);
    SDL_DestroyTexture(loadgameicon);

    SDL_Surface *leaderboardiconimg = IMG_Load("BACK.png");
    SDL_Texture *leaderboardicon = SDL_CreateTextureFromSurface(renderer,leaderboardiconimg);
    SDL_FreeSurface(leaderboardiconimg);
    pos.x = 350;
    pos.y = 600;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,leaderboardicon,NULL,&pos);
    SDL_DestroyTexture(leaderboardicon);


    SDL_RenderPresent(renderer);
}

void difcultnewgamemenu(){
    SDL_Surface *imgwallpaper = IMG_Load("wallpaper.jpg");
    SDL_Texture *wallpaper = SDL_CreateTextureFromSurface(renderer,imgwallpaper);
    SDL_FreeSurface(imgwallpaper);
    SDL_Rect pos = {.x = 0,.y = 0,realwidth,height};
    SDL_RenderCopy(renderer,wallpaper,NULL,&pos);
    SDL_DestroyTexture(wallpaper);

    SDL_Surface *logoimg = IMG_Load("LOGO.jpg");
    SDL_Texture *logo = SDL_CreateTextureFromSurface(renderer,logoimg);
    SDL_FreeSurface(logoimg);
    pos.x = 250;
    pos.y = 0;
    pos.w = 500;
    pos.h = 300;
    SDL_RenderCopy(renderer,logo,NULL,&pos);
    SDL_DestroyTexture(logo);

    SDL_Surface *newgameiconimg = IMG_Load("EASYBUTTON.png");
    SDL_Texture *newgameicon = SDL_CreateTextureFromSurface(renderer,newgameiconimg);
    SDL_FreeSurface(newgameiconimg);
    pos.x = 350;
    pos.y = 310;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,newgameicon,NULL,&pos);
    SDL_DestroyTexture(newgameicon);

    SDL_Surface *loadgameiconimg = IMG_Load("NORMALBUTTON.png");
    SDL_Texture *loadgameicon = SDL_CreateTextureFromSurface(renderer,loadgameiconimg);
    SDL_FreeSurface(loadgameiconimg);
    pos.x = 350;
    pos.y = 410;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,loadgameicon,NULL,&pos);
    SDL_DestroyTexture(loadgameicon);

    SDL_Surface *HARDgameiconimg = IMG_Load("HARDBUTTON.png");
    SDL_Texture *HARDgameicon = SDL_CreateTextureFromSurface(renderer,HARDgameiconimg);
    SDL_FreeSurface(HARDgameiconimg);
    pos.x = 350;
    pos.y = 510;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,HARDgameicon,NULL,&pos);
    SDL_DestroyTexture(HARDgameicon);


    SDL_Surface *leaderboardiconimg = IMG_Load("BACK.png");
    SDL_Texture *leaderboardicon = SDL_CreateTextureFromSurface(renderer,leaderboardiconimg);
    SDL_FreeSurface(leaderboardiconimg);
    pos.x = 350;
    pos.y = 610;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,leaderboardicon,NULL,&pos);
    SDL_DestroyTexture(leaderboardicon);


    SDL_RenderPresent(renderer);
}
void leaderboard(){
    char scoreline[30] = "";
    char scorenum[5] = "";
    SDL_Color color = {200,200,200,255};
    font = TTF_OpenFont("Lato-Italic.ttf",36);

    SDL_Surface *imgwallpaper = IMG_Load("wallpaper.jpg");
    SDL_Texture *wallpaper = SDL_CreateTextureFromSurface(renderer,imgwallpaper);
    SDL_FreeSurface(imgwallpaper);
    SDL_Rect pos = {.x = 0,.y = 0,realwidth,height};
    SDL_RenderCopy(renderer,wallpaper,NULL,&pos);
    SDL_DestroyTexture(wallpaper);

    SDL_Surface *scoretextimg1 = TTF_RenderText_Blended(font,"<vs Computer>",color);
    SDL_Texture *scoretext1 = SDL_CreateTextureFromSurface(renderer,scoretextimg1);
    SDL_FreeSurface(scoretextimg1);
    pos.x = 60 ;
    pos.y = 50 ;
    SDL_QueryTexture(scoretext1,NULL,NULL,&pos.w,&pos.h);
    SDL_RenderCopy(renderer,scoretext1,NULL,&pos);
    SDL_DestroyTexture(scoretext1);

    scoretextimg1 = TTF_RenderText_Blended(font,"<vs Player>",color);
    scoretext1 = SDL_CreateTextureFromSurface(renderer,scoretextimg1);
    SDL_FreeSurface(scoretextimg1);
    pos.x = 560 ;
    pos.y = 50 ;
    SDL_QueryTexture(scoretext1,NULL,NULL,&pos.w,&pos.h);
    SDL_RenderCopy(renderer,scoretext1,NULL,&pos);
    SDL_DestroyTexture(scoretext1);

    int j,u=0;char names[10][13],v[10],i=0,score[10];
    FILE *bS;
    for(u=0;u<2;u++){
        if(u==0){
            bS=fopen("Ranks1.bin","r");
        }else{
            bS=fopen("Ranks2.bin","r");
            i=0;
        }
        fread(&i,sizeof(char),1,bS);
        for(j=0;j<i;j++){
            fread(&v[j],sizeof(char),1,bS);
            fread(names+j,sizeof(char),v[j],bS);
            fread(&score[j],sizeof(char),1,bS);
            scoreline[0] = '\0';
            scorenum[0] = '\0';
            strncat(scoreline,names[j],v[j]);
            strcat(scoreline," : ");
            itoa(score[j],scorenum,10);
            strcat(scoreline,scorenum);

            SDL_Surface *scoretextimg1 = TTF_RenderText_Blended(font,scoreline,color);

            SDL_Texture *scoretext1 = SDL_CreateTextureFromSurface(renderer,scoretextimg1);

            SDL_FreeSurface(scoretextimg1);

            pos.x = 100 + u * 500;
            pos.y = 110 + j*60;

            SDL_QueryTexture(scoretext1,NULL,NULL,&pos.w,&pos.h);

            SDL_RenderCopy(renderer,scoretext1,NULL,&pos);

            SDL_DestroyTexture(scoretext1);

        }
        fclose(bS);
    }
    SDL_RenderPresent(renderer);
    font = TTF_OpenFont("Lato-Italic.ttf",23);

}
void displaygameresult(){
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer,0,0,0,150);
    SDL_RenderFillRect(renderer,NULL);


    char scoreline[30] = "CONGRATULATIONS ";
    SDL_Color color = {255,255,255,255};
    font = TTF_OpenFont("Lato-Italic.ttf",50);
        if(player2.points>player1.points){
            if(!computer){
                strcat(scoreline,player2.name);
                scores(player2.points,strlen(player2.name),player2.name);
            }
            else{
                strcat(scoreline,"Computer");
            }
        }else if(player2.points < player1.points){
            strcat(scoreline,player1.name);
            scores(player1.points,strlen(player1.name),player1.name);
        }else{
            scoreline[0] = '\0';
            strcat(scoreline,"WELL PLAYEd A TIE");

        }
    strcat(scoreline," !!!");
    SDL_Surface *scoretextimg1 = TTF_RenderText_Blended(font,scoreline,color);
    SDL_Texture *scoretext1 = SDL_CreateTextureFromSurface(renderer,scoretextimg1);
    SDL_FreeSurface(scoretextimg1);
    SDL_Rect pos = {.x = 100,.y = 150};
    SDL_QueryTexture(scoretext1,NULL,NULL,&pos.w,&pos.h);
    SDL_RenderCopy(renderer,scoretext1,NULL,&pos);
    SDL_DestroyTexture(scoretext1);





    scoretextimg1 = TTF_RenderText_Blended(font,"Press anywhere to return.",color);
    scoretext1 = SDL_CreateTextureFromSurface(renderer,scoretextimg1);
    SDL_FreeSurface(scoretextimg1);
    pos.x = 250;
    pos.y = 350;
    SDL_QueryTexture(scoretext1,NULL,NULL,&pos.w,&pos.h);
    SDL_RenderCopy(renderer,scoretext1,NULL,&pos);
    SDL_DestroyTexture(scoretext1);
    SDL_RenderPresent(renderer);
    font = TTF_OpenFont("Lato-Italic.ttf",23);
}
void nameinput(){
    font = TTF_OpenFont("Lato-Italic.ttf",40);

    SDL_Surface *imgwallpaper = IMG_Load("wallpaper.jpg");
    SDL_Texture *wallpaper = SDL_CreateTextureFromSurface(renderer,imgwallpaper);
    SDL_FreeSurface(imgwallpaper);
    SDL_Rect pos = {.x = 0,.y = 0,realwidth,height};
    SDL_RenderCopy(renderer,wallpaper,NULL,&pos);
    SDL_DestroyTexture(wallpaper);

    SDL_Surface *logoimg = IMG_Load("LOGO.jpg");
    SDL_Texture *logo = SDL_CreateTextureFromSurface(renderer,logoimg);
    SDL_FreeSurface(logoimg);
    pos.x = 250;
    pos.y = 0;
    pos.w = 500;
    pos.h = 300;
    SDL_RenderCopy(renderer,logo,NULL,&pos);
    SDL_DestroyTexture(logo);

    pos.x = 300;
    pos.y = 400;
    pos.w = 350;
    pos.h = 60;
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer,&pos);

    SDL_Color color = {200,200,200,255};
    SDL_Surface *scoretextimg1 = TTF_RenderText_Blended(font,"Player One :",color);
    SDL_Texture *scoretext1 = SDL_CreateTextureFromSurface(renderer,scoretextimg1);
    SDL_FreeSurface(scoretextimg1);
    pos.x = 100;
    pos.y = 400;
    SDL_QueryTexture(scoretext1,NULL,NULL,&pos.w,&pos.h);
    SDL_RenderCopy(renderer,scoretext1,NULL,&pos);
    SDL_DestroyTexture(scoretext1);

    imgwallpaper = IMG_Load("PLAY.png");
    wallpaper = SDL_CreateTextureFromSurface(renderer,imgwallpaper);
    SDL_FreeSurface(imgwallpaper);
    pos.x = 300;
    pos.y = 600;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,wallpaper,NULL,&pos);
    SDL_DestroyTexture(wallpaper);

    imgwallpaper = IMG_Load("BACK.png");
    wallpaper = SDL_CreateTextureFromSurface(renderer,imgwallpaper);
    SDL_FreeSurface(imgwallpaper);
    pos.x = 650;
    pos.y = 600;
    pos.w = 300;
    pos.h = 80;
    SDL_RenderCopy(renderer,wallpaper,NULL,&pos);
    SDL_DestroyTexture(wallpaper);

    if(!computer){
        pos.x = 300;
        pos.y = 500;
        pos.w = 350;
        pos.h = 60;
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderFillRect(renderer,&pos);

        SDL_Color color = {200,200,200,255};
        SDL_Surface *scoretextimg1 = TTF_RenderText_Blended(font,"Player Two :",color);
        SDL_Texture *scoretext1 = SDL_CreateTextureFromSurface(renderer,scoretextimg1);
        SDL_FreeSurface(scoretextimg1);
        pos.x = 100;
        pos.y = 500;
        SDL_QueryTexture(scoretext1,NULL,NULL,&pos.w,&pos.h);
        SDL_RenderCopy(renderer,scoretext1,NULL,&pos);
        SDL_DestroyTexture(scoretext1);

    }


    player1.name[0] = '\0';
    if(!computer)
        player2.name[0] = '\0';
    char tempname[20] = "";
    int p = 0;
    bool done = false;
    SDL_Event event;
    int mx,my;
    while( !done )
        {
            bool renderText = false;
            while( (SDL_PollEvent( &event ) != 0))
            {
                if( event.type == SDL_QUIT )
                {
                    killSDL();
                    quit = true;
                    done = true;
                    return;
                }

                else if(event.type == SDL_MOUSEBUTTONDOWN){
                    SDL_GetMouseState(&mx,&my);
                    if(mx<700 && mx>300 && my>500 && my<560){
                        SDL_StartTextInput();
                        strcpy(tempname,player2.name);
                        p = 2;
                    }
                    else if(mx<700 && mx>300 && my>400 && my<460){
                        SDL_StartTextInput();
                        strcpy(tempname,player1.name);
                        p = 1;
                    }
                    else if(mx>300 && mx<600 && my>600 && my<680)
                        done =true;
                    else if(mx>650 && mx<950 && my>600 && my<680){
                        if(computer)
                            return oneNewGame();
                        else
                            return twoNewGame();
                    }
                    else{
                        SDL_StopTextInput();
                        p = 0;
                    }
                }
                if(p){
                        if( event.type == SDL_KEYDOWN )
                        {
                            if( event.key.keysym.sym == SDLK_BACKSPACE && strlen(tempname) > 0 )
                            {
                                tempname[strlen(tempname)-1] = '\0';
                                renderText = true;
                            }
                            else if( event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
                            {
                                SDL_SetClipboardText(tempname);
                            }
                            else if( event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
                            {
                                strncat(tempname,SDL_GetClipboardText(),12-strlen(tempname));
                                renderText = true;
                            }
                        }
                        else if( event.type == SDL_TEXTINPUT )
                        {
                            if( !( SDL_GetModState() & KMOD_CTRL && ( event.text.text[ 0 ] == 'c' || event.text.text[ 0 ] == 'C' || event.text.text[ 0 ] == 'v' || event.text.text[ 0 ] == 'V' ) || strlen(tempname)>11 ) )
                            {
                                strncat(tempname,event.text.text,1);
                                renderText = true;
                            }
                        }
                }
            }
            if( renderText )
            {
                pos.x = 0;
                pos.y = 0;
                pos.w = realwidth;
                pos.h = height;
                SDL_RenderCopy(renderer,wallpaper,NULL,&pos);


                pos.x = 250;
                pos.y = 0;
                pos.w = 500;
                pos.h = 300;
                SDL_RenderCopy(renderer,logo,NULL,&pos);


                pos.x = 300;
                pos.y = 400;
                pos.w = 350;
                pos.h = 60;
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_RenderFillRect(renderer,&pos);

                color.r = 200;
                color.g = 200;
                color.b = 200;
                SDL_Surface *scoretextimg1 = TTF_RenderText_Blended(font,"Player One :",color);
                SDL_Texture *scoretext1 = SDL_CreateTextureFromSurface(renderer,scoretextimg1);
                SDL_FreeSurface(scoretextimg1);
                pos.x = 100;
                pos.y = 400;
                SDL_QueryTexture(scoretext1,NULL,NULL,&pos.w,&pos.h);
                SDL_RenderCopy(renderer,scoretext1,NULL,&pos);
                SDL_DestroyTexture(scoretext1);
                color.r = 0;
                color.g = 0;
                color.b = 0;
                if(!computer){
                    pos.x = 300;
                    pos.y = 500;
                    pos.w = 350;
                    pos.h = 60;
                    SDL_SetRenderDrawColor(renderer,255,255,255,255);
                    SDL_RenderFillRect(renderer,&pos);

                    SDL_Color color = {200,200,200,255};
                    SDL_Surface *scoretextimg1 = TTF_RenderText_Blended(font,"Player Two :",color);
                    SDL_Texture *scoretext1 = SDL_CreateTextureFromSurface(renderer,scoretextimg1);
                    SDL_FreeSurface(scoretextimg1);
                    pos.x = 100;
                    pos.y = 500;
                    SDL_QueryTexture(scoretext1,NULL,NULL,&pos.w,&pos.h);
                    SDL_RenderCopy(renderer,scoretext1,NULL,&pos);
                    SDL_DestroyTexture(scoretext1);

                }
                if(p == 1){
                    scoretextimg1 = TTF_RenderText_Blended(font,tempname,color);
                    scoretext1 = SDL_CreateTextureFromSurface(renderer,scoretextimg1);
                    SDL_FreeSurface(scoretextimg1);
                    pos.x = 310;
                    pos.y = 400;
                    SDL_QueryTexture(scoretext1,NULL,NULL,&pos.w,&pos.h);
                    SDL_RenderCopy(renderer,scoretext1,NULL,&pos);
                    SDL_DestroyTexture(scoretext1);
                    strcpy(player1.name,tempname);
                }else if(p == 2){
                    strcpy(player2.name,tempname);
                }

                scoretextimg1 = TTF_RenderText_Blended(font,player1.name,color);
                scoretext1 = SDL_CreateTextureFromSurface(renderer,scoretextimg1);
                SDL_FreeSurface(scoretextimg1);
                pos.x = 310;
                pos.y = 400;
                SDL_QueryTexture(scoretext1,NULL,NULL,&pos.w,&pos.h);
                SDL_RenderCopy(renderer,scoretext1,NULL,&pos);
                SDL_DestroyTexture(scoretext1);
                if(!computer){
                    scoretextimg1 = TTF_RenderText_Blended(font,player2.name,color);
                    scoretext1 = SDL_CreateTextureFromSurface(renderer,scoretextimg1);
                    SDL_FreeSurface(scoretextimg1);
                    pos.x = 310;
                    pos.y = 500;
                    SDL_QueryTexture(scoretext1,NULL,NULL,&pos.w,&pos.h);
                    SDL_RenderCopy(renderer,scoretext1,NULL,&pos);
                    SDL_DestroyTexture(scoretext1);
                }
                imgwallpaper = IMG_Load("PLAY.png");
                wallpaper = SDL_CreateTextureFromSurface(renderer,imgwallpaper);
                SDL_FreeSurface(imgwallpaper);
                pos.x = 300;
                pos.y = 600;
                pos.w = 300;
                pos.h = 80;
                SDL_RenderCopy(renderer,wallpaper,NULL,&pos);
                SDL_DestroyTexture(wallpaper);
            }
            SDL_RenderPresent(renderer);
			}
			SDL_StopTextInput();
			font = TTF_OpenFont("Lato-Italic.ttf",23);
}

void saveGame(int totalmoves,int dim,int AIworld[dim][dim],char array[dim][dim],int history[][7]){
    if(x==0){
        SDL_Event event;
        int mx,my;
        bool done = false;
        updateSave(true);
        done = false;
        while(!done){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                killSDL();
                quit = true;
                done = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&mx,&my);
                    if((mx>800 && mx<880) && (my>120 && my<170)){
                        sG = '1';
                        saved = fopen("saved1.bin","w");
                        x=1;
                        done = true;
                    }
                    else if(mx>900 && mx<980 && my<170 && my>120){
                        sG = '2';
                        saved = fopen("saved2.bin","w");
                        x=2;
                        done = true;
                    }
                    else if(mx>800 && mx<880 && my>180 && my<230){
                        sG = '3';
                        saved = fopen("saved3.bin","w");
                        x=3;
                        done = true;
                    }
                    else if(mx>900 && mx<980 && my>180 && my<230){
                        sG = '4';
                        saved = fopen("saved4.bin","w");
                        x=4;
                        done = true;
                    }
                    else if((mx>800 && mx<880) && my>240 && my<290){
                        sG = '5';
                        saved = fopen("saved5.bin","w");
                        x=5;
                        done = true;
                    }
                    else if(mx/100 == 7 && my/100 == 1){
                        return;
                    }
                    break;
            }

        }
    }else{
        if(sG=='5')
            saved = fopen("saved5.bin","w");
        else if(sG=='1')
            saved = fopen("saved1.bin","w");
        else if(sG=='2')
            saved = fopen("saved2.bin","w");
        else if(sG=='3')
            saved = fopen("saved3.bin","w");
        else if(sG=='4')
            saved = fopen("saved4.bin","w");
    }
    fwrite(&player,1,1,saved);
    fwrite(&computer,1,1,saved);
    fwrite(&dim,1,1,saved);
    fwrite(&totalmoves,1,1,saved);
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++)
            fwrite(&array[i][j],1,(dim)*(dim),saved);
    }
    fwrite(&player1.moves,1,1,saved);
    fwrite(&player2.moves,1,1,saved);
    fwrite(&maxmoves,1,1,saved);
    for(int i=0;i<maxmoves;i++){
        for(int j=0;j<7;j++)
            fwrite(&history[i][j],1,(dim)*(dim),saved);

    }
    if(computer){
        for(int i=0;i<dim;i++){
            for(int j=0;j<dim;j++)
                fwrite(&AIworld[i][j],1,(dim)*(dim),saved);
        }
    }
    char tempdif[10];
    itoa(((endtime - starttime)/1000)+diftime,tempdif,10);
    fwrite(&tempdif,sizeof(char),10,saved);
    int e = strlen(player1.name);
    int f = strlen(player2.name);
    fwrite(&e,sizeof(int),1,saved);fwrite(&player1.name,sizeof(char),e,saved);
    if(!computer){
        fwrite(&f,sizeof(int),1,saved);fwrite(&player2.name,sizeof(char),f,saved);
    }fwrite(&cI,1,1,saved);fwrite(&cO,1,1,saved);
    fclose(saved);
}

int scores(int point,int l,char name[l]){
    int j,u=0;char names[10][13],v[10],i=0,score[10]={0};
    FILE *lB;
    if(computer)
        lB = fopen("Ranks1.bin","r");
    else
        lB = fopen("Ranks2.bin","r");
    fread(&i,sizeof(char),1,lB);
    if(i!=0){
        for(j=0;j<i;j++){
            fread(&v[j],sizeof(char),1,lB);
            fread(names+j,sizeof(char),v[j],lB);
            fread(&score[j],sizeof(char),1,lB);
            if(point<=score[j])
                u++;
        }
        fclose(lB);
    }
    if(i!=9)
        i++;
    if(u!=9){
        if(computer)
            lB = fopen("Ranks1.bin","w");
        else
            lB = fopen("Ranks2.bin","w");
        fwrite(&i,sizeof(char),1,lB);
        for(j=0;j<u;j++){
            fwrite(&v[j],sizeof(char),1,lB);
            fwrite(names+j,sizeof(char),v[j],lB);
            fwrite(&score[j],sizeof(char),1,lB);
        }
        fwrite(&l,sizeof(char),1,lB);
        fwrite(name,sizeof(char),l,lB);
        fwrite(&point,sizeof(char),1,lB);
        for(j=u;j<i-1;j++){
            fwrite(&v[j],sizeof(char),1,lB);
            fwrite(names+j,sizeof(char),v[j],lB);
            fwrite(&score[j],sizeof(char),1,lB);
        }
    }
    fclose(lB);
    return;
}

void loadGame(){
    updateSave(false);
    FILE *load;
    SDL_MouseButtonEvent event;
    int mx,my;
    bool done = false;
    while(!done){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                    quit = true;
                    killSDL();

                    return;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button == SDL_BUTTON_LEFT){
                    SDL_GetMouseState(&mx,&my);
                    if(mx>190 && mx<490 && my>400 && my<480){
                        sG = '1';
                        done = true;
                        load = fopen("saved1.bin","r");
                        x=1;
                    }
                    else if(mx>510 && mx<810 && my>400 && my<480){
                        sG = '2';
                        done = true;
                        load = fopen("saved2.bin","r");
                        x=2;
                    }
                    else if(mx>190 && mx<490 && my>500 && my<580){
                        sG = '3';
                        done = true;
                        load = fopen("saved3.bin","r");
                        x=3;
                    }
                    else if(mx>510 && mx<810 && my>500 && my<580){
                        sG = '4';
                        done = true;
                        load = fopen("saved4.bin","r");
                        x=4;
                    }
                    else if(mx>190 && mx<490 && my>600 && my<680){
                        sG = '5';
                        done = true;
                        load = fopen("saved5.bin","r");
                        x=5;
                    }
                    else if(mx>510 && mx<810 && my>600 && my<680){
                        return main(NULL,NULL);
                    }
                    break;
            }
        }

    }
    fread(&player,1,1,load);
    fread(&computer,1,1,load);
    fread(&dim,1,1,load);
    fread(&totalmoves,1,1,load);
    int history[2 * (dim/2) * ((dim/2) + 1)][7];
    createhistory(dim,history);
    char world[dim][dim];
    int AIworld[dim][dim];
    createworld(dim,world);
    printf("\r");
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++)
            fread(&world[i][j],1,(dim)*(dim),load);

    }
    fread(&player1.moves,1,1,load);
    fread(&player2.moves,1,1,load);
    fread(&maxmoves,1,1,load);
    for(int i=0;i<maxmoves;i++){
        for(int j=0;j<7;j++)
            fread(&history[i][j],1,(dim)*(dim),load);

    }
    if(computer){
        for(int i=0;i<dim;i++){
            for(int j=0;j<dim;j++)
                fread(&AIworld[i][j],1,(dim)*(dim),load);
        }
    }
    char tempdif[10];
    fread(&tempdif,sizeof(char),10,load);
    diftime = atoi(tempdif);
    fread(&e,sizeof(int),1,load);fread(&player1.name,sizeof(char),e,load);
    if(!computer){
        fread(&f,sizeof(int),1,load);fread(&player2.name,sizeof(char),f,load);
    }fread(&cI,1,1,load);fread(&cO,1,1,load);
    fclose(load);
    if(totalmoves != 0){
        player1.points = history[totalmoves-1][4];
        player2.points = history[totalmoves-1][5];
    }
    int mx1 = 0, my1 = 0, mx2 = 0, my2 = 0;
    starttime = SDL_GetTicks();
    while(totalmoves<2*((dim/2)+1)*(dim/2) && !quit){
        if(totalmoves!=0)
            p = totalmoves;
        else
            p=1;
        update(world,mx1,my1);
        if(computer && (player == 2)){
            makeMove(dim,world,NULL,NULL,NULL,NULL,history,AIworld,computer,&player,&totalmoves,&maxmoves,&player1.points,&player2.points,&player1.moves,&player2.moves);
            SDL_Delay(100);
        }else{
        SDL_PollEvent(&event);
                switch(event.type){
                case SDL_QUIT:
                    quit = true;
                    killSDL();
                    return;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button == SDL_BUTTON_LEFT){
                        mouse = true;
                        SDL_GetMouseState(&mx1,&my1);
                        printf("\r");
                        if(mx1/100 == 7 && my1/100 == 0){
                            undo(dim,history,world,&totalmoves,&player1.points,&player2.points,&player1.moves,&player2.moves,&player);
                            while(computer && history[totalmoves][6] == 2)
                                undo(dim,history,world,&totalmoves,&player1.points,&player2.points,&player1.moves,&player2.moves,&player);
                        }
                        else if(mx1/100 == 9 && my1/100 == 0){
                            redo(dim,history,world,&totalmoves,maxmoves,&player1.points,&player2.points,&player1.moves,&player2.moves,&player);
                            while(computer && history[totalmoves][6] == 2 && totalmoves < maxmoves)
                                redo(dim,history,world,&totalmoves,maxmoves,&player1.points,&player2.points,&player1.moves,&player2.moves,&player);
                        }
                        else if(mx1/100 == 7 && my1/100 == 1){
                                saveGame(totalmoves,dim,AIworld,world,history);
                        }
                        else if(mx1 > 720 && my1/100 == 6){
                                return main(NULL,NULL);
                        }else
                            mouse = true;
                    }
                        break;
                case SDL_MOUSEBUTTONUP:
                    mouse = false;
                    if(event.button == SDL_BUTTON_LEFT){
                        SDL_GetMouseState(&mx2,&my2);
                    }
                    if(!((mx1/(height/dim))%2 || (my1/(width/dim))%2 || (mx2/(height/dim)%2 || (my2/(width/dim))%2))){
                        makeMove(dim,world,mx1/(height/dim),my1/(width/dim),mx2/(height/dim),my2/(width/dim),history,AIworld,computer,&player,&totalmoves,&maxmoves,&player1.points,&player2.points,&player1.moves,&player2.moves);
                    }
                    break;
                }
        }
    }
        update(world,mx1,my1);
        done = false;
        x=0;
        if(totalmoves==2*((dim/2)+1)*(dim/2)){
            if(player2.points>player1.points){
                if(!computer){
                    displaygameresult(player2.name);
                    scores(player2.points,strlen(player2.name),player2.name);
                }
                else{
                    displaygameresult("Computer");

                }
            }else{
                if(!computer){

                    displaygameresult(player1.name);
                }else{

                    displaygameresult(player1.name);
                }
                scores(player1.points,strlen(player1.name),player1.name);
            }
        }

        while(!done){
            while(SDL_PollEvent(&event) && !quit){
                switch(event.type){
                    case SDL_QUIT:
                        killSDL();
                        quit = true;
                        return;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        done = true;
                        return main(NULL,NULL);
                        break;
                }
            }
        }
}




void oneNewGame(){
    difcultnewgamemenu();
    SDL_MouseButtonEvent event;
    bool done = false;
    int mx1,my1;
    while(!done && !quit){
        while(SDL_WaitEvent(&event) && !done){
        switch(event.type){
            case SDL_QUIT:
                quit = true;
                killSDL();
                return;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button == SDL_BUTTON_LEFT){
                    SDL_GetMouseState(&mx1,&my1);

                    if(mx1>350 && mx1<650){
                    switch(my1/100){
                    case 3:
                        game = '1';
                        done = true;
                        break;
                    case 4:
                        game = '2';
                        done = true;
                        break;
                    case 5:
                        game = '3';
                        done = true;
                        break;
                    case 6:
                        game = '0';
                        done = true;
                    }
                    }
            }
        }
    }
    }
    computer = 1;

    switch(game){
        case '0':
            return newGame();
            break;
        case '1':
            do{
            nameinput();
            }while((strlen(player1.name) > 12 || strlen(player1.name) == 0) && !quit);
            dim = 7;
            break;
        case '2':
            do{
            nameinput();
            }while((strlen(player1.name) > 12 || strlen(player1.name) == 0) && !quit);
            dim = 11;
            break;
        case '3':
            do{
            nameinput();
            }while((strlen(player1.name) > 12 || strlen(player1.name) == 0) && !quit);
            dim = 15;
            break;
        default:
            return oneNewGame();

    }
}

void twoNewGame(){
    difcultnewgamemenu();
    SDL_MouseButtonEvent event;
    bool done = false;
    int mx1,my1;
    while(!done && !quit){
        while(SDL_WaitEvent(&event) && !done){
        switch(event.type){
            case SDL_QUIT:
                quit = true;
                killSDL();
                return;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button == SDL_BUTTON_LEFT){
                    SDL_GetMouseState(&mx1,&my1);
                    if(mx1>380 && mx1<610){
                    switch(my1/100){
                    case 3:
                        game = '1';
                        done = true;
                        break;
                    case 4:
                        game = '2';
                        done = true;
                        break;
                    case 5:
                        game = '3';
                        done = true;
                        break;
                    case 6:
                        game = '0';
                        done = true;
                    }
                    }
            }
        }
    }
    }

    switch(game){
        case '0':
            return newGame();
            break;
        case '1':
            do{
            nameinput();
            }while((strlen(player1.name) > 12 || strlen(player2.name) > 12 || strlen(player1.name) == 0 || strlen(player2.name) == 0 || !strcmp(player1.name,player2.name)) && !quit);
            dim = 7;
            break;
        case '2':
            do{
            nameinput();
            }while((strlen(player1.name) > 12 || strlen(player2.name) > 12 || strlen(player1.name) == 0 || strlen(player2.name) == 0 ||!strcmp(player1.name,player2.name)) && !quit);
            dim = 11;
            break;
        case '3':
            do{
            nameinput();
            }while((strlen(player1.name) > 12 || strlen(player2.name) > 12 || strlen(player1.name) == 0 || strlen(player2.name) == 0 || !strcmp(player1.name,player2.name)) && !quit);
            dim = 15;
            break;
        default:
            return twoNewGame();
            break;
    }
}

void newGame(){
    newgamemenu();
    SDL_MouseButtonEvent event;
    bool done = false;
    int mx1,my1;
    game = '0';
    while(!done && !quit){
        while(SDL_WaitEvent(&event) && !done){
        switch(event.type){
            case SDL_QUIT:
                quit = true;
                killSDL();
                return;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button == SDL_BUTTON_LEFT){
                    SDL_GetMouseState(&mx1,&my1);
                    if(mx1>350 && mx1<650){
                    switch(my1/100){
                        case 4:
                            game = '1';
                            done = true;
                            break;
                        case 5:
                            game = '2';
                            done = true;
                            break;
                        case 6:
                            game = '0';
                            done = true;
                            break;
                    }
                }
                }
            }
        }
    }
    switch(game){
    case '0':
        return main(NULL,NULL);
        break;
    case '1':
        computer = 1;
        oneNewGame();
        break;
    case '2':
        computer = 0;
        twoNewGame();
        break;
    default:
        return newGame();
        break;
    }
}

void settings(){
    bool done = false,sets = false;
    int mx1,my1;
    while(!done && !quit){
        sets = false;
        SDL_Rect pos = {.x = 0,.y = 0,realwidth,height};
        SDL_RenderCopy(renderer,wallpaper,NULL,&pos);

        SDL_Surface *logoimg = IMG_Load("LOGO.jpg");
        SDL_Texture *logo = SDL_CreateTextureFromSurface(renderer,logoimg);
        SDL_FreeSurface(logoimg);
        pos.x = 250;
        pos.y = 0;
        pos.w = 500;
        pos.h = 300;
        SDL_RenderCopy(renderer,logo,NULL,&pos);
        SDL_DestroyTexture(logo);

        SDL_Surface *soundbuttonimg = IMG_Load("togglesound - Copy.png");
        if(cII){SDL_FreeSurface(soundbuttonimg);soundbuttonimg = IMG_Load("togglesound.png");}
        SDL_Texture *soundbutton = SDL_CreateTextureFromSurface(renderer,soundbuttonimg);
        SDL_FreeSurface(soundbuttonimg);
        pos.x = 400;
        pos.y = 320;
        pos.w = 200;
        pos.h = 60;
        SDL_RenderCopy(renderer,soundbutton,NULL,&pos);
        SDL_DestroyTexture(soundbutton);

        SDL_Surface *settings11iconimg = IMG_Load("settings11 - Copy.png");
        if(cI){SDL_FreeSurface(settings11iconimg);settings11iconimg = IMG_Load("settings11.png");}
        SDL_Texture *settings11icon = SDL_CreateTextureFromSurface(renderer,settings11iconimg);
        SDL_FreeSurface(settings11iconimg);
        pos.x = 190;
        pos.y = 400;
        pos.w = 300;
        pos.h = 80;
        SDL_RenderCopy(renderer,settings11icon,NULL,&pos);
        SDL_DestroyTexture(settings11icon);

        SDL_Surface *settings12iconimg = IMG_Load("settings12.png");
        if(cI){SDL_FreeSurface(settings11iconimg);settings12iconimg = IMG_Load("settings12 - Copy.png");}
        SDL_Texture *settings12icon = SDL_CreateTextureFromSurface(renderer,settings12iconimg);
        SDL_FreeSurface(settings12iconimg);
        pos.x = 510;
        pos.y = 400;
        pos.w = 300;
        pos.h = 80;
        SDL_RenderCopy(renderer,settings12icon,NULL,&pos);
        SDL_DestroyTexture(settings12icon);

        SDL_Surface *settings21iconimg = IMG_Load("settings21 - Copy.png");
        if(cO){SDL_FreeSurface(settings21iconimg);settings21iconimg = IMG_Load("settings21.png");}
        SDL_Texture *settings21icon = SDL_CreateTextureFromSurface(renderer,settings21iconimg);
        SDL_FreeSurface(settings21iconimg);
        pos.x = 190;
        pos.y = 500;
        pos.w = 300;
        pos.h = 80;
        SDL_RenderCopy(renderer,settings21icon,NULL,&pos);
        SDL_DestroyTexture(settings21icon);

        SDL_Surface *settings22iconimg = IMG_Load("settings22.png");
        if(cO){SDL_FreeSurface(settings22iconimg);settings22iconimg = IMG_Load("settings22 - Copy.png");}
        SDL_Texture *settings22icon = SDL_CreateTextureFromSurface(renderer,settings22iconimg);
        SDL_FreeSurface(settings22iconimg);
        pos.x = 510;
        pos.y = 500;
        pos.w = 300;
        pos.h = 80;
        SDL_RenderCopy(renderer,settings22icon,NULL,&pos);
        SDL_DestroyTexture(settings22icon);

        SDL_Surface *backiconimg = IMG_Load("BACK.png");
        SDL_Texture *backicon = SDL_CreateTextureFromSurface(renderer,backiconimg);
        SDL_FreeSurface(backiconimg);
        pos.x = 350;
        pos.y = 600;
        pos.w = 300;
        pos.h = 80;
        SDL_RenderCopy(renderer,backicon,NULL,&pos);
        SDL_DestroyTexture(backicon);

        SDL_RenderPresent(renderer);

        SDL_MouseButtonEvent event;
        while(SDL_WaitEvent(&event) && !done && !sets){
        switch(event.type){
            case SDL_QUIT:
                quit = true;
                killSDL();
                done = true;
                sets = true;
                return;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button == SDL_BUTTON_LEFT){
                    SDL_GetMouseState(&mx1,&my1);
                    if(mx1>350 && mx1<650 && my1>600 && my1<680)
                        done = true;
                    else if(mx1>190 && mx1<490 && my1>400 && my1<480){cI=0;sets=true;}
                    else if(mx1>510 && mx1<810 && my1>400 && my1<480){cI=1;sets=true;}
                    else if(mx1>190 && mx1<490 && my1>500 && my1<580){cO=0;sets=true;}
                    else if(mx1>510 && mx1<810 && my1>500 && my1<580){cO=1;sets=true;}
                    else if(mx1>400 && mx1<600 && my1>320 && my1<380 && !cII){cII=1;Mix_PauseMusic();sets=true;}
                    else if(mx1>400 && mx1<600 && my1>320 && my1<380 && cII){cII=0;Mix_PlayMusic(backgroundmusic,-1);sets=true;}
                }
        }
        }
    }
    return main(NULL,NULL);
}

int main(int argc,char* argv[]){
    HWND console;
    AllocConsole();
    console = FindWindowA("ConsoleWindowClass",NULL);
    ShowWindow(console,0);
    do{
    if(!SDLrun){
        initSDL();
        SDLrun = true;
    }
    totalmoves = 0;player1.moves=0;player1.points=0;
    maxmoves = 0;player2.moves=0;player2.points=0;
    x = 0;
    diftime = 0;
    gamemenu();
    SDL_MouseButtonEvent event;
    int mx1 = 0, my1 = 0, mx2 = 0, my2 = 0;
    bool done = false;
    while(!done && !quit){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                quit = true;
                killSDL();
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button == SDL_BUTTON_LEFT){
                    SDL_GetMouseState(&mx1,&my1);

                    if(my1>400 && my1<480 && mx1>190 && mx1<490){
                        game = '1';
                        done = true;
                    }else if(my1>400 && my1<480 && mx1>510 && mx1<810){
                        game = '2';
                        done = true;
                    }else if(my1>500 && my1<580 && mx1>190 && mx1<490){
                        game = '3';
                        done = true;
                    }else if(my1>500 && my1<580 && mx1>510 && mx1<810){
                        game = '4';
                        done = true;
                    }else if(my1>600 && my1<680 && mx1>350 && mx1<650){
                        game = '0';
                        done = true;
                    }break;
            }
        }
    }

    switch(game){
    case '1':{
        do{
            newGame();
        }while(dim == 0 && !quit);
        if(quit)
            return 0;
        int history[2 * (dim/2) * ((dim/2) + 1)][7];
        createhistory(dim,history);

        char world[dim][dim];
        createworld(dim,world);

        int AIworld[dim][dim];
        createAIwolrd(dim,AIworld);

        SDL_MouseButtonEvent click;

        totalmoves = 0;
        player = 1;
        starttime = SDL_GetTicks();
        ran = false;
        while(totalmoves<2*((dim/2)+1)*(dim/2) && !quit){
            if(totalmoves!=0)
                p = totalmoves;
            else
                p=1;
            update(world,mx1,my1);

            ran = true;
            if(computer && (player == 2)){
                makeMove(dim,world,NULL,NULL,NULL,NULL,history,AIworld,computer,&player,&totalmoves,&maxmoves,&player1.points,&player2.points,&player1.moves,&player2.moves);
                SDL_Delay(100);
            }else{
            SDL_PollEvent(&click);
                    switch(click.type){
                    case SDL_QUIT:
                            quit = true;
                            killSDL();
                            return 0;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if(click.button == SDL_BUTTON_LEFT){
                            SDL_GetMouseState(&mx1,&my1);
                            printf("\r");
                            if(mx1/100 == 7 && my1/100 == 0){
                                undo(dim,history,world,&totalmoves,&player1.points,&player2.points,&player1.moves,&player2.moves,&player);
                                while(computer && history[totalmoves][6] == 2)
                                    undo(dim,history,world,&totalmoves,&player1.points,&player2.points,&player1.moves,&player2.moves,&player);

                            }
                            else if(mx1/100 == 9 && my1/100 == 0){
                                redo(dim,history,world,&totalmoves,maxmoves,&player1.points,&player2.points,&player1.moves,&player2.moves,&player);
                                while(computer && history[totalmoves][6] == 2 && totalmoves < maxmoves)
                                    redo(dim,history,world,&totalmoves,maxmoves,&player1.points,&player2.points,&player1.moves,&player2.moves,&player);


                            }
                            else if(mx1/100 == 7 && my1/100 == 1){
                                saveGame(totalmoves,dim,AIworld,world,history);
                                SDL_Delay(90);
                                if(quit)
                                    return 0;
                            }
                            else if(mx1 > 720 && my1/100 == 6){
                                    return main(NULL,NULL);

                            }else
                                mouse = true;
                        }
                            break;
                    case SDL_MOUSEBUTTONUP:
                        if(!mouse)
                            break;
                        mouse = false;
                        if(click.button == SDL_BUTTON_LEFT){
                            SDL_GetMouseState(&mx2,&my2);
                        }
                        if(!((mx1/(height/dim))%2 || (my1/(width/dim))%2 || (mx2/(height/dim)%2 || (my2/(width/dim))%2))){
                            makeMove(dim,world,mx1/(height/dim),my1/(width/dim),mx2/(height/dim),my2/(width/dim),history,AIworld,computer,&player,&totalmoves,&maxmoves,&player1.points,&player2.points,&player1.moves,&player2.moves);
                        }

                        break;

                    }

            }

        }
        update(world,mx1,my1);
        x=0;
        displaygameresult();
        done = false;
        while(!done){
        while(SDL_WaitEvent(&event) && !quit && !done){
            switch(event.type){
                case SDL_QUIT:
                        killSDL();
                        quit = true;
                        done = true;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        done = true;
                        return main(NULL,NULL);
                        break;
                }
            }
        }

        break;
    }
    case '2':
        loadGame();
        if(quit){
            return;
        }
        break;
    case '3':
        leaderboard();
        done = false;
        while(!done){
            while(SDL_WaitEvent(&event) && !quit && !done){
                switch(event.type){
                    case SDL_QUIT:
                        done = true;
                        killSDL();
                        quit = true;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        done = true;
                        break;
                }
            }
        }
        break;
    case '4':
        settings();
        break;
    case '0':
        quit = true;
        killSDL();
        break;
    }
    }while(!quit);
    return 0;
}
