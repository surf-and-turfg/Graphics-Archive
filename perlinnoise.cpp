#include <SDL2/SDL.h>  
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

using namespace std;

constexpr int width = 640;
constexpr int height = 640;
constexpr int limit = 255;

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
vector<float> constvec(float v){
    int h = static_cast<int>(v) & 3;
    if (h==0){
        return {1.0f,1.0f};
    }else if (h==1){
        return {-1.0f,1.0f};
    }else if (h==2){
        return {-1.0f,-1.0f};
    }else{
        return {1.0f,-1.0f};
    }

}

float lerp(float t,float a1, float a2){
    return a1 + t *(a2-a1);
}

float fadeddenahoe(float t){
    return 6*t*t*t*t*t - 15*t*t*t*t + 10*t*t*t;
}

float gennoise(float x, float y, vector<float>* wrapper){
    int X = static_cast<int>(floor(x)) & limit; 
    int Y = static_cast<int>(floor(y)) & limit;  
    float xf = x - static_cast<float>(floor(x));
    float yf = y - static_cast<float>(floor(y));
    float topright[2] = {xf-1.0f,yf-1.0f};
    float topleft[2] = {xf,yf-1.0f};
    float bottomright[2] = {xf-1.0f,yf};
    float bottomleft[2] = {xf,yf};

    float valuetr = (*wrapper)[(X + 1) & limit] + ((Y + 1) & limit);
    float valuetl = (*wrapper)[X & limit] + ((Y + 1) & limit);
    float valuebr = (*wrapper)[(X + 1) & limit] + (Y & limit);
    float valuebl = (*wrapper)[X & limit] + (Y & limit);
 
    vector<float> temp = constvec(valuetr);
    float dottr = topright[0] * temp[0] + topright[1] * temp[1];

    temp = constvec(valuetl);
    float dottl = topleft[0] * temp[0]+topleft[1] * temp[1];

    temp = constvec(valuebr);
    float dotbr = bottomright[0] * temp[0]+bottomright[1] * temp[1];

    temp = constvec(valuebl);
    float dotbl = bottomleft[0] * temp[0]+bottomleft[1] * temp[1];

    float u = fadeddenahoe(xf);
    float v = fadeddenahoe(yf);
    return lerp(u,lerp(v,dotbl,dottl),lerp(v,dotbr,dottr));
}



int main(void) {
    SDL_Event event;
    //random garbage off of the internet
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
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
    vector<float> wrapper;
    for (int x = 0; x<255; x++){
        wrapper.push_back(x);
    }
    random_device rd;
    mt19937 generator(rd());
    shuffle(wrapper.begin(), wrapper.end(), generator);
    int t_length = 600;
    int t_width = 600;
    auto texture = SDL_CreateTexture(renderer, 
                  SDL_PIXELFORMAT_RGBA8888, 
                  SDL_TEXTUREACCESS_STREAMING, 
                  t_length, 
                  t_width);

    uint8_t *pixels;
    int pitch;
    //game loop
    while (running){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_LockTexture(texture, NULL, (void **)&pixels, &pitch);

        for(int x = 0; x< 600; x++){
            for (int y = 0; y < 600; y++){
                float num = gennoise(static_cast<float>(x) * 0.005f, static_cast<float>(y)* 0.005f,&wrapper);
                num = (num + 1.0f) * (255.0f/2.0f);
                int pixelIndex = ((y) * pitch) + ((x) * 4);
                pixels[pixelIndex] = 255;        // alpha component
                pixels[pixelIndex + 1] = static_cast<int>(255.0f*(num/255.0f));    //blue component
                pixels[pixelIndex + 2] = static_cast<int>((255.0f/2.0f)*(num/255.0f));    // green component
                pixels[pixelIndex + 3] = static_cast<int>((255.0f/1.5f)*(num/255.0f));    // red component

                //cout << gennoise(static_cast<float>(x) * 0.005f, static_cast<float>(y)* 0.005f,&wrapper) << endl;
            }
        }   
        SDL_UnlockTexture(texture);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        process(event,window,renderer,&running);
    }
    return 0;
}

