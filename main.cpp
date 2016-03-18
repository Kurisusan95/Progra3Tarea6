#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<stdio.h>
#include<iostream>
#include<list>
#include "Character.h"
#include "Button.h"
#include "Button.h"
#include "DeviceButton.h"
#include "InputManager.h"

using namespace std;


SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background,*gameover,*gameover2;
SDL_Rect rect_background,rect_game_over,rect_game_over2,rect_character;
int vida=1;
int vida2=1;

void loopJuego(string fondo)
{

    //Init textures
    background = IMG_LoadTexture(renderer,fondo.c_str());
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = 1200;
    rect_background.h = 600;

    vida = 1;
    SDL_Texture *barra_vida[6];
    barra_vida[0] = IMG_LoadTexture(renderer,"ProgressBar1.png");
    barra_vida[1] = IMG_LoadTexture(renderer,"ProgressBar2.png");
    barra_vida[2] = IMG_LoadTexture(renderer,"ProgressBar3.png");
    barra_vida[3] = IMG_LoadTexture(renderer,"ProgressBar4.png");
    barra_vida[4] = IMG_LoadTexture(renderer,"ProgressBar5.png");
    barra_vida[5] = IMG_LoadTexture(renderer,"ProgressBar6.png");
    SDL_Rect vida_rect;
    SDL_QueryTexture(barra_vida[0], NULL, NULL, &vida_rect.w, &vida_rect.h);
    vida_rect.x = 30;
    vida_rect.y = 30;
    vida_rect.w = 250;
    vida_rect.h = 47;

    vida2 = 1;
    SDL_Texture *barra_vida2[6];
    barra_vida2[0] = IMG_LoadTexture(renderer,"ProgressBar1.png");
    barra_vida2[1] = IMG_LoadTexture(renderer,"ProgressBar2.png");
    barra_vida2[2] = IMG_LoadTexture(renderer,"ProgressBar3.png");
    barra_vida2[3] = IMG_LoadTexture(renderer,"ProgressBar4.png");
    barra_vida2[4] = IMG_LoadTexture(renderer,"ProgressBar5.png");
    barra_vida2[5] = IMG_LoadTexture(renderer,"ProgressBar6.png");
    SDL_Rect vida2_rect;
    SDL_QueryTexture(barra_vida2[0], NULL, NULL, &vida2_rect.w, &vida2_rect.h);
    vida2_rect.x = 900;
    vida2_rect.y = 30;
    vida2_rect.w = 250;
    vida2_rect.h = 47;

    Character* character = new Character(renderer,200,550,false,"assets/inputs_player1.txt","assets/joystick_player1.txt");
    Character* character2 = new Character(renderer,800,550,true,"assets/inputs_player2.txt","assets/joystick_player2.txt");

    map<DeviceButton*,Button*>input_map;

    int frame=0;

    double last_fame_ticks=SDL_GetTicks();

    SDL_JoystickOpen( 0 );

    //Main Loop
    while(true)
    {
        frame++;
//        while(SDL_PollEvent(&Event))
//        {
//            if(Event.type == SDL_QUIT)
//            {
//                return 0;
//            }
//            if(Event.type == SDL_KEYDOWN)
//            {
//                if(Event.key.keysym.sym == SDLK_d)
//                    rect_character.x++;
//                if(Event.key.keysym.sym == SDLK_a)
//                    rect_character.x--;
//            }
//        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);


        SDL_RenderCopy(renderer, background, NULL, &rect_background);
        SDL_RenderCopy(renderer,barra_vida[vida-1],NULL,&vida_rect);
        SDL_RenderCopy(renderer,barra_vida2[vida2-1],NULL,&vida2_rect);

        character->logic();
        character->draw();

        character2->logic();
        character2->draw();

        vector<Hitbox*>char1_hitboxes=character->getHitboxes();
        vector<Hitbox*>char2_hitboxes=character2->getHitboxes();

        vector<Hitbox*>char1_hurtboxes=character->getHurtboxes();
        vector<Hitbox*>char2_hurtboxes=character2->getHurtboxes();

        for(int i=0;i<char1_hitboxes.size();i++)
        {
            for(int j=0;j<char2_hurtboxes.size();j++)
            {
                SDL_Rect rect1 = char1_hitboxes[i]->rect;
                SDL_Rect rect2 = char2_hurtboxes[j]->rect;

                if(!character->flipped)
                {
                    rect1.x+=character->x;
                }else
                {
                    rect1.x=-rect1.x-rect1.w+character->x;
                }
                rect1.y=-rect1.y+character->y;

                if(!character2->flipped)
                {
                    rect2.x+=character2->x;
                }else
                {
                    rect2.x=-rect2.x-rect2.w+character2->x;
                }
                rect2.y=-rect2.y+character2->y;
                if(collides(rect1,rect2))
                {
                    cout<<"Jugador 1 conecto un golpe"<<endl;
                    character2->cancel("on_hit");
                }else
                {
                    //cout<<"No Colision!"<<endl;
                }
            }
        }

        for(int i=0;i<char1_hurtboxes.size();i++)
        {
            for(int j=0;j<char2_hitboxes.size();j++)
            {
                SDL_Rect rect1 = char1_hurtboxes[i]->rect;
                SDL_Rect rect2 = char2_hitboxes[j]->rect;

                if(!character->flipped)
                {
                    rect1.x+=character->x;
                }else
                {
                    rect1.x=-rect1.x-rect1.w+character->x;
                }
                rect1.y=-rect1.y+character->y;

                if(!character2->flipped)
                {
                    rect2.x+=character2->x;
                }else
                {
                    rect2.x=-rect2.x-rect2.w+character2->x;
                }
                rect2.y=-rect2.y+character2->y;
                if(collides(rect1,rect2))
                {
                    cout<<"Jugador 2 conecto un golpe"<<endl;
                    character->cancel("on_hit");
                }else
                {
                    //cout<<"No Colision!"<<endl;
                }
            }
        }

        if(character->x>character2->x)
        {
            character->flipped=true;
            character2->flipped=false;
        }else
        {
            character->flipped=false;
            character2->flipped=true;
        }

        double last_frame_length=SDL_GetTicks()-last_fame_ticks;
        double sleep_time=17-last_frame_length;
        //cout<<"Length: "<<last_frame_length<<", Sleep time: "<<sleep_time<<endl;
        if(sleep_time>0)
            SDL_Delay(sleep_time);
        last_fame_ticks=SDL_GetTicks();

        if(character->x<=0)
        {
            cout<<"Jugador 2 ha ganado"<<endl;
            character->x=200;
            character2->x=800;
            vida++;
            if(vida<6)
                SDL_RenderCopy(renderer,barra_vida[vida-1],NULL,&vida_rect);
            else
                return;
        }

        if(character2->x>=1000)
        {
            cout<<"Jugador 1 ha ganado"<<endl;
            character->x=200;
            character2->x=800;
            vida2++;
            if(vida2<6)
                SDL_RenderCopy(renderer,barra_vida2[vida2-1],NULL,&vida2_rect);
            else
                return;
        }


//        drawRect(renderer,10,30,50,100,
//                 0,255,255,0);

        SDL_RenderPresent(renderer);
    }

	return;
}
void mainMenu()
{
    int opcion = 1;
    SDL_Texture *menu_fondo[3];
    menu_fondo[0] = IMG_LoadTexture(renderer,"strootFighter_fondo1.png");
    menu_fondo[1] = IMG_LoadTexture(renderer,"strootFighter_fondo2.png");
    menu_fondo[2] = IMG_LoadTexture(renderer,"strootFighter_fondo3.png");
    SDL_Rect menu_rect;

    SDL_QueryTexture(menu_fondo[0], NULL, NULL, &menu_rect.w, &menu_rect.h);
    menu_rect.x = 0;
    menu_rect.y = 0;

    gameover = IMG_LoadTexture(renderer,"game_Over.png");
    rect_game_over.x = 0;
    rect_game_over.y = 0;
    rect_game_over.w = 1200;
    rect_game_over.h = 600;

    gameover2 = IMG_LoadTexture(renderer,"game_Over2.png");
    rect_game_over2.x = 0;
    rect_game_over2.y = 0;
    rect_game_over2.w = 1200;
    rect_game_over2.h = 600;

    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_2)
                {
                    return;
                }
                if(Event.key.keysym.sym == SDLK_1)
                {
                    loopJuego("fondo.png");
                }
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
                if(Event.key.keysym.sym == SDLK_RIGHT)
                {
                    opcion++;
                    if(opcion > 3)
                        opcion = 3;
                }
                if(Event.key.keysym.sym == SDLK_LEFT)
                {
                    opcion--;
                    if(opcion < 1)
                        opcion = 1;
                }
                if(Event.key.keysym.sym == SDLK_RETURN)
                {
                    switch(opcion)
                    {
                        case 1:
                            if(vida<6 && vida2<6)
                                loopJuego("fondo.png");
                        break;
                        case 2:
                            if(vida<6 && vida2<6)
                                loopJuego("fondo1.png");
                        break;
                        case 3:
                            if(vida<6 && vida2<6)
                                loopJuego("fondo2.png");
                        break;
                    }
                }
            }
        }
        SDL_RenderCopy(renderer,menu_fondo[opcion-1],NULL,&menu_rect);
        if(vida==6)
            SDL_RenderCopy(renderer, gameover2, NULL, &rect_game_over2);
        else if(vida2==6)
            SDL_RenderCopy(renderer, gameover, NULL, &rect_game_over);

        SDL_RenderPresent(renderer);

    }
}
int main( int argc, char* args[] )
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, 1200/*WIDTH*/, 600/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    mainMenu();

	return 0;
}

