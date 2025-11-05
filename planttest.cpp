#include <SDL2/SDL.h>  
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <set>
using namespace std;
constexpr double rotamt = 60.0;
constexpr double pi = 3.14;

double to_rad(double num){
    return (num / 180) * pi;  
}

void process(SDL_Event &event, SDL_Window* window, SDL_Renderer* renderer, bool* running){
    //function for processing game input
    while( SDL_PollEvent( &event ) ){
        switch( event.type ){
            //put different keys in here
            case SDL_KEYDOWN:
                case SDLK_ESCAPE:
                    (*running) = false;
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    break;
                break;
            case SDL_KEYUP:
                break;
            default:
                break;
      }
   }
}

void generate(int maxdepth, int depth, double rotation,string curpos, unordered_map<string,set<pair<int,int>>>* grid){
    if (depth > maxdepth){
        return;
    }
    int newx = 0;
    int newy = 20;
    int swap = curpos.find(",");
    int x = stoi(curpos.substr(0,swap));
    int y = stoi(curpos.substr(swap+1));
    rotation += rotamt;
    int rotnewx = newx * cos(to_rad(rotation)) - newy * sin(to_rad(rotation));
    int rotnewy = newy * cos(to_rad(rotation)) + newx * sin(to_rad(rotation));
    rotnewx +=x;
    rotnewy +=y;
    (*grid)[curpos].insert(make_pair(rotnewx,rotnewy));
    generate(maxdepth, depth+1,rotation,to_string(rotnewx)+","+to_string(rotnewy),grid);
    rotnewx = x-rotnewx + x;
    (*grid)[curpos].insert(make_pair(rotnewx,rotnewy));
    generate(maxdepth, depth+1,rotation,to_string(rotnewx)+","+to_string(rotnewy),grid);
    return;
}


int main(void) {
    SDL_Event event;
    //random garbage off of the internet
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 1;
    }

    bool running = true;
    unordered_map<string,set<pair<int,int>>> grid;
    generate(15,0,108.0,"300,300",&grid);
    //game loop
    while (running){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for ( auto i : grid){
            for (auto j : i.second){
                int swap = (i.first).find(",");
                int x = stoi((i.first).substr(0,swap));
                int y = stoi((i.first).substr(swap+1));
                SDL_RenderDrawLine(renderer, x, y, j.first, j.second);
            }
        }
        SDL_RenderPresent(renderer);
        process(event,window,renderer,&running);
    }



    return 0;
}
        

