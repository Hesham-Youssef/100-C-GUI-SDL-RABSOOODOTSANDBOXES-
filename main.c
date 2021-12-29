#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <string.h>
#define realwidth 1000
#define width 700
#define height 700
//#include "undo and redo.h"
int e=0,f=0,p,x=0,playerturn = 0, player = 1,moves[2] = {0},totalmoves=0,maxmoves = 0,points[2] = {0},dim=0,computer;char game=0,name1[25],name2[25]="Computer",ss[1],sG;
bool mouse = false;FILE *saved;
SDL_Window *window;
SDL_Renderer *renderer;


void initSDL(){
    SDL_Init(SDL_INIT_VIDEO);

    IMG_Init(IMG_INIT_PNG | IMG_INIT_PNG);

    window = SDL_CreateWindow("dots and boxes",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,realwidth,height,SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

}

void killSDL(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}


void update(char world[dim][dim],int mx1,int my1){
    int shift,L,wz,hv,e,f,linex,liney;
    float a,b,c,d;
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
    SDL_Surface *saveimg = IMG_Load("saveicon.png");

    SDL_Texture *saveicon = SDL_CreateTextureFromSurface(renderer,saveimg);

    SDL_FreeSurface(saveimg);

    SDL_Rect savepos = {.x = 710,.y = 120,.w = 100,.h = 100};

    SDL_Surface *imgredo = IMG_Load("redo.png");

    SDL_Texture *redoicon = SDL_CreateTextureFromSurface(renderer,imgredo);

    SDL_FreeSurface(imgredo);

    SDL_Rect redopos = {.x = 900,.y = 20,.w = 80,.h = 80};

    SDL_Surface *imgundo = IMG_Load("undo.png");

    SDL_Texture *undoicon = SDL_CreateTextureFromSurface(renderer,imgundo);

    SDL_FreeSurface(imgundo);

    SDL_Rect undopos = {.x = 720,.y = 20,.w = 80,.h = 80};

    SDL_Surface *imgwallpaper = IMG_Load("wallpaper.jpg");

    SDL_Texture *wallpaper = SDL_CreateTextureFromSurface(renderer,imgwallpaper);

    SDL_FreeSurface(imgwallpaper);

    SDL_Surface *imagedots = IMG_Load("dots.png");

    SDL_Texture *dots = SDL_CreateTextureFromSurface(renderer,imagedots);

    SDL_FreeSurface(imagedots);

    SDL_SetRenderDrawColor(renderer,0,0,0,255);

    SDL_RenderClear(renderer);

    SDL_Rect wallpaperpos = {.x = 0,.y = 0,realwidth,height};

    SDL_RenderCopy(renderer,wallpaper,NULL,&wallpaperpos);

    SDL_RenderCopy(renderer,redoicon,NULL,&redopos);

    SDL_RenderCopy(renderer,undoicon,NULL,&undopos);

    SDL_RenderCopy(renderer,saveicon,NULL,&savepos);

    if(mouse){
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_GetMouseState(&linex,&liney);
        SDL_RenderDrawLine(renderer,mx1,my1,linex,liney);
    }
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            if(world[i][j] == '0' || world[i][j] == '1'){

                SDL_SetRenderDrawColor(renderer,180,180,180,180);

                if((i%2 && !(j%2)) && world[i][j] == '1'){ // HORIZONTAL
                    SDL_Rect ballrect = {.x = (i-a) * width/dim + shift , .y = (j) * height/dim + shift , .w = wz , .h = L};
                    SDL_RenderFillRect(renderer,&ballrect); // VERTICAL
                }else if((!(i%2) && (j%2) )&& world[i][j] == '1'){
                    SDL_Rect ballrect = {.x = (i) * width/dim + shift , .y = (j-b) * height/dim + shift , .w = L , .h = hv};
                    SDL_RenderFillRect(renderer,&ballrect);
                }

            }
            else if(world[i][j] == 'X'){

                SDL_SetRenderDrawColor(renderer,255,0,0,255);
                SDL_Rect ballrect = {.x = (i-a+c) * width/dim + shift , .y = (j-b+d) * height/dim + shift , .w = wz - e , .h = hv - f};

                SDL_RenderFillRect(renderer,&ballrect);
            }
            else if(world[i][j] == 'O'){

                SDL_SetRenderDrawColor(renderer,0,0,255,255);
                SDL_Rect ballrect = {.x = (i-a+c) * width/dim + shift , .y = (j-b+d) * height/dim + shift , .w = wz - e , .h = hv - f};

                SDL_RenderFillRect(renderer,&ballrect);
            }
        }
    }
    for(int i=0;i<dim;i=i+2){
        for(int j=0;j<dim;j=j+2){
            SDL_Rect dotspos = {.x = i * width/dim + shift , .y = j * height/dim + shift , .w = L , .h = L};
            SDL_RenderCopy(renderer,dots,NULL,&dotspos);
        }
    }
    SDL_DestroyTexture(undoicon);
    SDL_DestroyTexture(redoicon);
    SDL_DestroyTexture(wallpaper);
    SDL_DestroyTexture(dots);
    SDL_RenderPresent(renderer);

}

void updatesave(){
    SDL_Surface *save1iconimg = IMG_Load("1saveimg.png");
    SDL_Texture *save1icon = SDL_CreateTextureFromSurface(renderer,save1iconimg);
    SDL_FreeSurface(save1iconimg);
    SDL_Rect save1pos = {.x = 800,.y = 120,.w = 120,.h = 80};
    SDL_RenderCopy(renderer,save1icon,NULL,&save1pos);


    SDL_Surface *save2iconimg = IMG_Load("2saveimg.png");
    SDL_Texture *save2icon = SDL_CreateTextureFromSurface(renderer,save2iconimg);
    SDL_FreeSurface(save2iconimg);
    SDL_Rect save2pos = {.x = 900,.y = 120,.w = 120,.h = 80};
    SDL_RenderCopy(renderer,save2icon,NULL,&save2pos);

    SDL_Surface *save3iconimg = IMG_Load("3saveimg.png");
    SDL_Texture *save3icon = SDL_CreateTextureFromSurface(renderer,save3iconimg);
    SDL_FreeSurface(save3iconimg);
    SDL_Rect save3pos = {.x = 800,.y = 180,.w = 120,.h = 80};
    SDL_RenderCopy(renderer,save3icon,NULL,&save3pos);

    SDL_Surface *save4iconimg = IMG_Load("4saveimg.png");
    SDL_Texture *save4icon = SDL_CreateTextureFromSurface(renderer,save4iconimg);
    SDL_FreeSurface(save4iconimg);
    SDL_Rect save4pos = {.x = 900,.y = 180,.w = 120,.h = 80};
    SDL_RenderCopy(renderer,save4icon,NULL,&save4pos);

    SDL_Surface *save5iconimg = IMG_Load("5saveimg.png");
    SDL_Texture *save5icon = SDL_CreateTextureFromSurface(renderer,save5iconimg);
    SDL_FreeSurface(save5iconimg);
    SDL_Rect save5pos = {.x = 800,.y = 230,.w = 120,.h = 80};
    SDL_RenderCopy(renderer,save5icon,NULL,&save5pos);



    SDL_DestroyTexture(save5icon);
    SDL_DestroyTexture(save4icon);
    SDL_DestroyTexture(save3icon);
    SDL_DestroyTexture(save2icon);
    SDL_DestroyTexture(save1icon);
    SDL_RenderPresent(renderer);
}






void printAIwolrd(int dim,int AIworld[dim][dim]){
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            printf("%d  ",AIworld[i][j]);
        }
        printf("\n");
    }
}
void createAIwolrd(int dim,int AIworld[dim][dim]){
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            AIworld[i][j] = 0;
        }
    }
}


void printhistory(int dim,int history[][7]){
    for(int i=0;i<maxmoves;i++){
        for(int j=0;j<7;j++){
            printf("  %d -",history[i][j]);
        }
        printf("\n");
    }
}


void printworld(int dim,char array[dim][dim]){
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            printf("%c  ",array[i][j]);
        }
        printf("\n");
    }
}
void createworld(int dim,char array[dim][dim]){
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            if(!(j%2 || i%2))
                array[i][j]='0';
            else
                array[i][j]=' ';
        }
    }
}
void createhistory(int dim,int history[2 * (dim/2) * ((dim/2) + 1)][7]){
    for(int i=0;i<2 * (dim/2) * ((dim/2) + 1);i++){
        for(int j=0;j<7;j++){
            history[i][j] = 0;
        }
    }
}

int checkforotherlines(int dim,int history[][7],char array[][dim],int n1,int m1){
    int f=0;
    if(array[n1][m1+1] == '1')
        f++;
    if(array[n1][m1-1] == '1')
        f++;
    if(array[n1+1][m1] == '1')
        f++;
    if(array[n1-1][m1] == '1')
        f++;
    return f;
}




int upperright(int n1,int m1,int dim,char array[dim][dim]){
    if((m1 == dim -1) || (n1 == 0) || array[n1-1][m1+1] == 'X' || array[n1-1][m1+1] == 'O')
        return 0;
    if(array[n1][m1+1]=='1'){
            if(array[n1-1][m1]=='1'){
                if(array[n1-1][m1+2]=='1'){
                    if(array[n1-2][m1+1]=='1'){
                        switch(player){
                            case 1:
                                array[n1-1][m1+1] = 'X';
                                break;
                            case 2:

                                array[n1-1][m1+1] = 'O';
                                break;
                        }
                        return 1;
                    }
                }
            }
    }
   return 0;
}
int upperleft(int n1,int m1,int dim,char array[dim][dim]){
    if((m1 == 0) || (n1 == 0) || array[n1-1][m1-1] == 'X' || array[n1-1][m1-1] == 'O')
        return 0;
    if(array[n1][m1-1]=='1'){
            if(array[n1-1][m1]=='1'){
                if(array[n1-1][m1-2]=='1'){
                    if(array[n1-2][m1-1]=='1'){
                        switch(player){
                            case 1:
                                array[n1-1][m1-1] = 'X';
                                break;
                            case 2:

                                array[n1-1][m1-1] = 'O';
                                break;
                        }
                        return 1;
                    }
                }
            }
    }
    return 0;
}
int downright(int n1,int m1,int dim,char array[dim][dim]){
    if((m1 == dim -1) || (n1 == dim -1) || array[n1+1][m1+1] == 'X' || array[n1+1][m1+1] == 'O')
        return 0;
    if(array[n1][m1+1]=='1'){
            if(array[n1+1][m1]=='1'){
                if(array[n1+1][m1+2]=='1'){
                    if(array[n1+2][m1+1]=='1'){
                        switch(player){
                            case 1:

                                array[n1+1][m1+1] = 'X';
                                break;
                            case 2:

                                array[n1+1][m1+1] = 'O';
                                break;
                        }
                        return 1;
                    }
                }
            }
    }
    return 0;
}
int downleft(int n1,int m1,int dim,char array[dim][dim]){
    if((m1 == 0) || (n1 == dim-1) || array[n1+1][m1-1] == 'X' || array[n1+1][m1-1] == 'O')
        return 0;
    if(array[n1][m1-1]=='1'){
            if(array[n1+1][m1]=='1'){
                if(array[n1+1][m1-2]=='1'){
                    if(array[n1+2][m1-1]=='1'){
                        switch(player){
                            case 1:

                                array[n1+1][m1-1] = 'X';
                                break;
                            case 2:

                                array[n1+1][m1-1] = 'O';
                                break;
                        }
                        return 1;
                    }
                }
            }
    }
    return 0;
}
void checkforsquares(int n1,int m1,int dim,char array[dim][dim],int history[][7],int points[]){
    int sum=0;
    sum = upperright(n1,m1,dim,array) + upperleft(n1,m1,dim,array) + downright(n1,m1,dim,array) + downleft(n1,m1,dim,array);
    if(playerturn % 2)
        points[1] += sum;
    else
        points[0] += sum;
    playerturn++;
    switch(player){
        case 1:
            moves[0]++;
            break;
        case 2:
            moves[1]++;
    }
    if(sum != 0)
        playerturn++;
    else{
        player=player==1?2:1;
    }

}

void undo(int dim,int history[][7],char array[dim][dim]){
    if(totalmoves > 0){
        totalmoves--;
        int n1 = history[totalmoves][0] , m1 = history[totalmoves][1] ,  n2 = history[totalmoves][2] ,  m2 = history[totalmoves][3];
        array[(history[totalmoves][0]+history[totalmoves][2])/2][(history[totalmoves][1]+history[totalmoves][3])/2] =' ';
        if(!(checkforotherlines(dim,history,array,history[totalmoves][0],history[totalmoves][1]) >= 1))
            array[history[totalmoves][0]][history[totalmoves][1]] = '0';

        if(!(checkforotherlines(dim,history,array,history[totalmoves][2],history[totalmoves][3]) >= 1))
            array[history[totalmoves][2]][history[totalmoves][3]] = '0';

        if(n1==n2){
            if(n1==0)
                array[n1+1][(m1+m2)/2]=' ';
            else if(n1==dim-1)
                array[n1-1][(m1+m2)/2]=' ';
            else{
                array[n1+1][(m1+m2)/2]=' ';
                array[n1-1][(m1+m2)/2]=' ';
            }
        }else{
            if(m1==0)
                array[(n1+n2)/2][m1+1]=' ';
            else if(n1==dim-1)
                array[(n1+n2)/2][m1-1]=' ';
            else{
                array[(n1+n2)/2][m1+1]=' ';
                array[(n1+n2)/2][m1-1]=' ';
            }
        }
        if(player==history[totalmoves][6]){
            playerturn-=2;
            switch(player){
            case 1:
                points[0]--;
                break;
            case 2:
                points[1]--;
            }
        }
        else
            playerturn--;
        player = history[totalmoves][6];
        switch(player){
            case 1:
                moves[0]--;
                break;
            case 2:
                moves[1]--;
                break;
        }
    }else{
        printf("How do you think it is supposed to undo IF YOU HAVEN'T PLAYED YET!\a\n");

    }
}

void redo(int dim,int history[][7],char array[dim][dim]){
    if(totalmoves < maxmoves && (maxmoves > 1)){
        int n1 = history[totalmoves][0];int m1 = history[totalmoves][1];
        array[(history[totalmoves][0]+history[totalmoves][2])/2][(history[totalmoves][1]+history[totalmoves][3])/2] ='1';

        array[history[totalmoves][0]][history[totalmoves][1]] = '1';

        array[history[totalmoves][2]][history[totalmoves][3]] = '1';

        checkforsquares(n1,m1,dim,array,history,points);
        totalmoves++;

    }else{
        printf("How do you think it is supposed to redo IF THERE IS NO UNDO MOVES!\a\n");

    }
}

void saveGame(int totalmoves,int dim,int AIworld[dim][dim],char array[dim][dim],int history[][7]){
    SDL_MouseButtonEvent event;
    int mx,my;
    bool done = false;
    if(x==0){
        updatesave();
        done = false;
        while(!done){
        SDL_PollEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                killSDL();
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button == SDL_BUTTON_LEFT){
                    SDL_GetMouseState(&mx,&my);
                    if(mx/100 == 8 && my/100 == 1){
                        sG = '1';
                        done = true;
                    }
                    else if(mx/100 == 9 && my/100 == 1){
                        sG = '2';
                        done = true;
                    }
                    else if(mx/100 == 8 && my/100 == 2 && my%100 < 50){
                        sG = '3';
                        done = true;
                    }
                    else if(mx/100 == 9 && my/100 == 2){
                        sG = '4';
                        done = true;
                    }
                    else if(mx/100 == 8 && my/100 == 2 && my%100 > 50){
                        sG = '5';
                        done = true;
                    }
                    else if(mx/100 == 7 && my/100 == 1)
                        return;
                    }
                    break;
            }
        }
        printf("\nChoose a file to save(1,2,3,4,5) or press any key to return:");
        if(sG=='5'){
            saved = fopen("saved5.bin","w");
            x=5;
        }
        else if(sG=='1'){
            saved = fopen("saved1.bin","w");
            x=1;
        }
        else if(sG=='2'){
            saved = fopen("saved2.bin","w");
            x=2;
        }
        else if(sG=='3'){
            saved = fopen("saved3.bin","w");
            x=3;
        }
        else if(sG=='4'){
            saved = fopen("saved4.bin","w");
            x=4;
        }
        else{
            printf("\nenter a valid thing");
            return;
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
    fwrite(&computer,sizeof(int),1,saved);
    fwrite(&dim,sizeof(int),1,saved);
    fwrite(&totalmoves,sizeof(int),1,saved);
    for(int i=0;i<totalmoves;i++){
        for(int j=0;j<7;j++)
            fwrite(&history[i][j],1,(dim)*(dim),saved);

    }
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++)
            fwrite(&array[i][j],1,(dim)*(dim),saved);

    }
    for(int i=0;i<2;i++)
        fwrite(&moves[i],sizeof(int),2,saved);
    fwrite(&playerturn,sizeof(int),1,saved);
    fwrite(&maxmoves,sizeof(int),1,saved);

    if(computer){
        for(int i=0;i<dim;i++){
            for(int j=0;j<dim;j++)
                fwrite(&AIworld[i][j],1,(dim)*(dim),saved);
        }
    }

    fwrite(strlen(name1),sizeof(int),1,saved);fwrite(&name1,sizeof(char),strlen(name1),saved);
    if(!computer)
        fwrite(strlen(name2),sizeof(int),1,saved);fwrite(&name2,sizeof(char),strlen(name2),saved);

    fclose(saved);
    printf("\nGame saved in %d",x);
}







void loadGame(){
    FILE *load;
    printf("Choose a file (1,2,3,4,5) or press any key to return");
    sG = _getche();
    system("cls");
    if(sG=='5'){
        load = fopen("saved5.bin","r");
        x=5;
    }
    else if(sG=='1'){
        load = fopen("saved1.bin","r");
        x=1;
    }
    else if(sG=='2'){
        load = fopen("saved2.bin","r");
        x=2;
    }
    else if(sG=='3'){
        load = fopen("saved3.bin","r");
        x=3;
    }
    else if(sG=='4'){
        load = fopen("saved4.bin","r");
        x=4;
    }
    else
        return main(NULL,NULL);
    fread(&computer,sizeof(int),1,load);
    fread(&dim,sizeof(int),1,load);
    fread(&totalmoves,sizeof(int),1,load);
    int history[2 * (dim/2) * ((dim/2) + 1)][7];
    initSDL();
    SDL_MouseButtonEvent event;
    char world[dim][dim];
    int AIworld[dim][dim];
    for(int i=0;i<totalmoves;i++){
        for(int j=0;j<7;j++)
            fread(&history[i][j],1,(dim)*(dim),load);

    }
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++)
            fread(&world[i][j],1,(dim)*(dim),load);

    }
    for(int i=0;i<2;i++)
        fread(&moves[i],sizeof(int),2,load);
    fread(&playerturn,sizeof(int),1,load);
    fread(&maxmoves,sizeof(int),1,load);
    if(computer){
        for(int i=0;i<dim;i++){
            for(int j=0;j<dim;j++)
                fread(&AIworld[i][j],1,(dim)*(dim),load);
        }
    }
    fread(strlen(name1),sizeof(int),1,load);fread(&name1,sizeof(char),strlen(name1),load);
    if(!computer)
        fread(strlen(name2),sizeof(int),1,load);fread(&name2,sizeof(char),strlen(name2),load);
    fclose(load);
    int mx1 = 0, my1 = 0, mx2 = 0, my2 = 0;
    bool quit = false;
    while(totalmoves<2*((dim/2)+1)*(dim/2) && !quit){
        if(totalmoves!=0)
            p = totalmoves;
        else
            p=1;
        update(world,mx1,my1);
        if(computer && (player == 2)){
            makeamove(dim,world,NULL,NULL,NULL,NULL,points,history,AIworld);
        }else{
        while(SDL_PollEvent(&event)){
                switch(event.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button == SDL_BUTTON_LEFT)
                        mouse = true;
                        SDL_GetMouseState(&mx1,&my1);
                        if(mx1/100 == 7 && my1/100 == 0){
                            undo(dim,history,world);
                            while(computer && history[totalmoves][6] == 2)
                                undo(dim,history,world);
                        }
                        else if(mx1/100 == 9 && my1/100 == 0){
                            redo(dim,history,world);
                            while(computer && history[totalmoves][6] == 2)
                                redo(dim,history,world);
                        }
                        else if(mx1/100 == 7 && my1/100 == 1)
                                saveGame(totalmoves,dim,AIworld,world,history);
                        break;
                case SDL_MOUSEBUTTONUP:
                    mouse = false;
                    if(event.button == SDL_BUTTON_LEFT){
                        SDL_GetMouseState(&mx2,&my2);
                    }
                    if(!((mx1/(height/dim))%2 || (my1/(width/dim))%2 || (mx2/(height/dim)%2 || (my2/(width/dim))%2))){
                        makeamove(dim,world,mx1/(height/dim),my1/(width/dim),mx2/(height/dim),my2/(width/dim),points,history,AIworld);
                    }
                    break;
                }
            }
        }
    }
    system("cls");
    update(world,mx1,my1);
    while(SDL_WaitEvent(&event)){
        if(event.type == SDL_MOUSEBUTTONDOWN)
            killSDL();
    }
    x=0;
    if(totalmoves==2*((dim/2)+1)*(dim/2)){
        if(points[1]>points[0]){
            if(!computer){
                printf("Congratulation for %s and hard luck for %s\n",name2,name1);
                scores(points[1],f,name2);
            }
            else
                printf("Hard luck %s\n",name1);
        }else{
            if(!computer)
                printf("Congratulations for %s and hard luck for %s\n",name1,name2);
            else
                printf("Congratulations %s\n",name1);
            scores(points[0],e,name1);
        }
    }
    printf("Press any key to proceed\n");
    ss[0]=_getch();
}



void makeamove(int dim,char array[dim][dim],int n1,int m1,int n2,int m2,int points[],int history[][7],int AIworld[dim][dim]){

    if(!computer && player == 2 || player == 1){

        if((array[((n1+n2)/2)][((m1+m2)/2)] =='1') || (n1 > dim - 1) || (m1 > dim - 1) || (n2 > dim - 1) || (m2 > dim - 1) || (n1 < 0) || (m1 < 0) || (n2 < 0) || (m2 < 0) || !(((abs(n1-n2) == 2) && (m1==m2)) ^ ((abs(m1-m2) == 2) && (n1==n2)))){
            printf("enter a valid move\n");
            return;
        }
        history[totalmoves][0] = n1 ; history[totalmoves][1] = m1 ;history[totalmoves][2] = n2 ; history[totalmoves][3] = m2;

    }else{
        AI(dim,array,AIworld,history);
    }

    history[totalmoves][6] = player;


    if(!(history[totalmoves][1]%2 || history[totalmoves][0]%2 )){
        array[history[totalmoves][0]][history[totalmoves][1]] = '1';
        array[history[totalmoves][2]][history[totalmoves][3]] = '1';
        array[(history[totalmoves][0]+history[totalmoves][2])/2][(history[totalmoves][1]+history[totalmoves][3])/2] ='1';
    }
    else{
        printf("enter a valid thing");
    }

    checkforsquares(history[totalmoves][0],history[totalmoves][1],dim,array,history,points);
    history[totalmoves][4] = points[0];
    history[totalmoves][5] = points[1];

    if((totalmoves < maxmoves) && (!computer && player == 2))
        maxmoves = totalmoves;

        maxmoves++;
        totalmoves++;
}

void AI(int dim,char world[dim][dim],int AIworld[dim][dim],int history[][7]){
    int numberoflines1,numberoflines2;
    // checks horizontal lines
    for(int i=0;i<dim;i=i+2){
        for(int j=1;j<dim;j=j+2){
            numberoflines1 = 0;
            numberoflines2 = 0;
            if(world[i][j] == '1')
                AIworld[i][j] = -100;
            else{
                if( i != 0){
                    if(world[i-1][j+1] == '1')
                         numberoflines1++;
                    if(world[i-1][j-1] == '1')
                         numberoflines1++;
                    if(world[i-2][j] == '1')
                         numberoflines1++;
                    if(numberoflines1 == 3)
                         numberoflines1 += 10;
                    else if(numberoflines1 == 2)
                        numberoflines1 -=10;
                }
                if( i != dim - 1){
                    if(world[i+1][j-1] == '1')
                         numberoflines2++;
                    if(world[i+1][j+1] == '1')
                         numberoflines2++;
                    if(world[i+2][j] == '1')
                         numberoflines2++;
                    if(numberoflines2 == 3)
                        numberoflines2 +=10;
                    else if(numberoflines2 == 2)
                        numberoflines2 -=10;
                }
                AIworld[i][j] = numberoflines1 + numberoflines2;
            }
        }
    }
    //checks vertical lines
    for(int i=1;i<dim;i=i+2){
        for(int j=0;j<dim;j=j+2){
            numberoflines1 = 0;
            numberoflines2 = 0;
            if(world[i][j] == '1')
                AIworld[i][j] = -100;
            else{
                if( j != 0){
                    if(world[i-1][j-1] == '1')
                         numberoflines1++;
                    if(world[i+1][j-1] == '1')
                         numberoflines1++;
                    if(world[i][j-2] == '1')
                         numberoflines1++;
                    if(numberoflines1 == 3)
                         numberoflines1 += 10;
                    else if(numberoflines1 == 2)
                        numberoflines1 -=10;
                }
                if( j != dim-1){
                    if(world[i+1][j+1] == '1')
                         numberoflines2++;
                    if(world[i-1][j+1] == '1')
                         numberoflines2++;
                    if(world[i][j+2] == '1')
                         numberoflines2++;
                    if(numberoflines2 == 3)
                        numberoflines2 +=10;
                    else if(numberoflines2 == 2)
                        numberoflines2 -=10;
                }
                AIworld[i][j] = numberoflines1 + numberoflines2;
            }
        }
    }
    int maxi,maxj,max=-100;
    for(int i=1;i<dim;i=i+2){
        for(int j=0;j<dim;j=j+2){
            if(AIworld[i][j] > max){
                max = AIworld[i][j];
                maxi = i;
                maxj = j;
            }
        }
    }
    for(int i=0;i<dim;i=i+2){
        for(int j=1;j<dim;j=j+2){
            if(AIworld[i][j] > max){
                max = AIworld[i][j];
                maxi = i;
                maxj = j;
            }
        }
    }
    if(!(maxi%2)){
        history[totalmoves][0] = maxi;
        history[totalmoves][1] = maxj-1;
        history[totalmoves][2] = maxi;
        history[totalmoves][3] = maxj+1;
    }else{
        history[totalmoves][0] = maxi-1;
        history[totalmoves][1] = maxj;
        history[totalmoves][2] = maxi+1;
        history[totalmoves][3] = maxj;

    }


}

int scores(int point,int l,char name[l]){
    int j,y[10],u=0,t;char names[10][25],v[10],i=0,score[10]={0};
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
    printf("Press any key to return to menu\n");
    ss[0]=_getch();
    system("cls");
    return main(NULL,NULL);
}




void oneNewGame(){
    computer = 1;
    system("cls");
    printf("Back(0)\n\nEnter difficulty:\nEasy(1)\nNormal(2)\nHard(3)\nVery hard(4)\n");
    game = _getch();
    switch(game){
        case '0':
            return newGame();
            break;
        case '1':
            printf("\nEnter player's name: ");
            gets(name1);
            dim = 7;
            break;
        case '2':
            printf("\nEnter player's name: ");
            gets(name1);
            dim = 11;
            break;
        case '3':
            printf("\nEnter player's name: ");
            gets(name1);
            dim = 11;
            break;
        case '4':
            printf("\nEnter player's name: ");
            gets(name1);
            dim = 15;
            break;
        default:
            oneNewGame();
    }
}

void twoNewGame(){
    system("cls");
    printf("Back(0)\n\nEnter difficulty:\nEasy(1)\nNormal(2)\nHard(3)\n");
    game = _getch();
    switch(game){
        case '0':
            return newGame();
            break;
        case '1':
            printf("\nEnter first player's name: ");
            gets(name1);
            printf("\nEnter second player's name: ");
            gets(name2);
            dim = 7;
            break;
        case '2':
            printf("\nEnter first player's name: ");
            gets(name1);
            printf("\nEnter second player's name: ");
            gets(name2);
            dim = 11;
            break;
        case '3':
            printf("\nEnter first player's name: ");
            gets(name1);
            printf("\nEnter second player's name: ");
            gets(name2);
            dim = 15;
            break;
        default:
            twoNewGame();
            break;
    }
}

void newGame(){
    system("cls");
    printf("Back(0)\n\nChoose a mode:\nVs Computer(1)\nVs Human(2)\n");
    game = _getch();
    switch(game){
    case '0':
        system("cls");
        main(NULL,NULL);
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
        newGame();
        break;
    }
}



int main(int argc,char* argv[]){
    do{
    system("color f1");
    printf("Welcome to dots and boxes by RABSOOO team\nNew game(1)\nLoad game(2)\nLeader board(3)\nSettings(4)\nExit(0)\n");
    game = _getch();
    system("cls");
    bool quit = false;
    switch(game){
    case '1':{
        system("color f0");
        do{
            newGame();
        }

        while(dim == 0);

        int history[2 * (dim/2) * ((dim/2) + 1)][7];
        char world[dim][dim];

        system("color f4");
        createworld(dim,world);


        int AIworld[dim][dim];

        int temp[dim][dim];

        createAIwolrd(dim,AIworld);

        createhistory(dim,history);

        initSDL();


        SDL_MouseButtonEvent event;


        int mx1 = 0, my1 = 0, mx2 = 0, my2 = 0;

        totalmoves = 0;
        player = 1;
        while(totalmoves<2*((dim/2)+1)*(dim/2) && !quit){
            if(totalmoves!=0)
                p = totalmoves;
            else
                p=1;




            update(world,mx1,my1);

            if(computer && (player == 2)){
                makeamove(dim,world,NULL,NULL,NULL,NULL,points,history,AIworld);

            }else{
            while(SDL_PollEvent(&event)){
                    int f = 0;
                    switch(event.type){
                    case SDL_QUIT:
                        quit = true;
                        killSDL();
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if(event.button == SDL_BUTTON_LEFT)
                            mouse = true;
                            SDL_GetMouseState(&mx1,&my1);
                            if(mx1/100 == 7 && my1/100 == 0){
                                undo(dim,history,world);
                                while(computer && history[totalmoves][6] == 2)
                                    undo(dim,history,world);
                            }
                            else if(mx1/100 == 9 && my1/100 == 0){
                                redo(dim,history,world);
                                while(computer && history[totalmoves][6] == 2)
                                    redo(dim,history,world);
                            }
                            else if(mx1/100 == 7 && my1/100 == 1){
                                saveGame(totalmoves,dim,AIworld,world,history);
                            }
                            break;
                    case SDL_MOUSEBUTTONUP:
                        mouse = false;
                        if(event.button == SDL_BUTTON_LEFT){
                            SDL_GetMouseState(&mx2,&my2);
                        }
                        if(!((mx1/(height/dim))%2 || (my1/(width/dim))%2 || (mx2/(height/dim)%2 || (my2/(width/dim))%2))){
                            makeamove(dim,world,mx1/(height/dim),my1/(width/dim),mx2/(height/dim),my2/(width/dim),points,history,AIworld);
                        }
                        break;

                    }
                }
            }
        }

        system("cls");
        update(world,mx1,my1);
        while(SDL_WaitEvent(&event)){
            if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_QUIT)
                killSDL();
        }
        x=0;
        if(totalmoves==2*((dim/2)+1)*(dim/2)){
            if(points[1]>points[0]){
                if(!computer){
                    printf("Congratulation for %s and hard luck for %s\n",name2,name1);
                    scores(points[1],f,name2);
                }
                else
                    printf("Hard luck %s\n",name1);
            }else{
                if(!computer)
                    printf("Congratulations for %s and hard luck for %s\n",name1,name2);
                else
                    printf("Congratulations %s\n",name1);
                scores(points[0],e,name1);
            }
        }
        printf("Press any key to proceed\n");
        ss[0]=_getch();
        break;
        scanf("%c",&argv);
    }
    case '2':
        system("cls");
        loadGame();
        break;
    case '3':
        int j,y,u=0;char names[10][25],v[10],i=0,score[10];
        FILE *bS;
        for(u=0;u<2;u++){
            if(u==0){
                bS=fopen("Ranks1.bin","r");
                printf("Press any key to return\n\nOne player mode\n\n");
            }else{
                bS=fopen("Ranks2.bin","r");
                printf("\nTwo player mode\n\n");
                i=0;
            }
            fread(&i,sizeof(char),1,bS);
            for(j=0;j<i;j++){
                fread(&v[j],sizeof(char),1,bS);
                fread(names+j,sizeof(char),v[j],bS);
                fread(&score[j],sizeof(char),1,bS);
                printf("%d) ",j+1);
                for(y=0;y<v[j];y++)
                    printf("%c",names[j][y]);
                printf(": %d\n",score[j]);
            }
            fclose(bS);
        }
        ss[0]=_getch();
        system("cls");
        break;
    case '4':
    case '0':
        system("cls");
        printf("thanks for playing :)\n");
        break;
    default:
        system("cls");
        printf("Enter a valid input\n");
        main(NULL,NULL);
    }
    }while(game < 1 || game >5);



    return 0;
}
