#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 640;

typedef struct
{
    int r;
    int g;
    int b;
    int a;
}coorColor;

void fill_rect(SDL_Renderer* renderer, int x, int y, int width, int height, coorColor color)
{
    SDL_Rect rect = { 0 };
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}


int main(int argc, char* args[]) {
    // Se inicializan todos los subsistemas
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Error al inicializar SDL: %s\n", SDL_GetError());
    }
    SDL_Window* window = SDL_CreateWindow("TestTetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 640, 0);
    SDL_Delay(2000);




    //Llama al programa que controla la tarjeta grafica y setea flags
    uint32_t render_flags = SDL_RENDERER_ACCELERATED;
    uint32_t render_flags1 = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(window, -1, render_flags);
    SDL_Renderer* rend1 = SDL_CreateRenderer(window, -1, render_flags1);
    SDL_Renderer* rendwindow = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(rendwindow, 255, 0, 0, 255);
    SDL_RenderClear(rendwindow);
    SDL_RenderPresent(rendwindow);
    SDL_Delay(2000);


    // Crea un surface para cargar imagen dentro del main memory
    SDL_Surface* surface;

    //path para la imagen
    surface = IMG_Load("/Users/jackfeels/Desktop/TETRIS/TEST XCODE/2TEST/2TEST/assets/png dino/Dead (4).png");

    //carga la imagen en la memoria de la tarjeta grafica
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);

    //limpia el main memory
    SDL_FreeSurface(surface);

    //Deja controlar la imagen para moverlo con el keyboard
    SDL_Rect dest;

    //conecta la texture con dest para controlar la posicion
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

    //ajusta la altura h y el ancho w de la imagen
    dest.w /= 6;
    dest.h /= 6;

    //setea la posicion inicial en el eje x
    dest.x = (480 - dest.w) / 2;
    //setea la posicion inicial en el eje y
    dest.y = (640 - dest.h) / 2;

    //controla el loop de la animacion
    //bool close = true;

    //Velocidad del image box
    int speed = 300;

    int close = 0;

    while (!close)
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
                // Para eliminar la ventana
            case SDL_QUIT:
                close = 1;
                break;
            case SDL_KEYDOWN:

                //funciones y scaneo de teclas para movimiento
                switch (ev.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    dest.y -= speed / 30;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    dest.x -= speed / 30;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    dest.y += speed / 30;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    dest.x += speed / 30;
                    break;
                default:
                    break;
                }
            }
        }
        // limites de movimiento
        // limit derecho
        if (dest.x + dest.w > 480)
        {
            dest.x = 480 - dest.w;
        }
        //limit izquierdo
        if (dest.x < 0)
        {
            dest.x = 0;
        }
        //limit abajo
        if (dest.y + dest.h > 640)
        {
            dest.y = 640 - dest.h;
        }
        //limit arriba
        if (dest.y < 0)
        {
            dest.y = 0;
        }
        //rend1 = SDL_CreateRenderer(window,-1,0);
        SDL_SetRenderDrawColor(rend1, 160, 160, 160, 255);
        //SDL_RenderClear(rend1);

        // Crea cuadrado
        SDL_Rect fillRect = { 0, 0, 48 * 2, 40 * 2 };
        SDL_SetRenderDrawColor(rend1, 255, 255, 0, 255);
        SDL_RenderFillRect(rend1, &fillRect);


        // 16 Lineas horizontales
        for (int i = 1; i < 17; i++) {
            SDL_SetRenderDrawColor(rend1, 0, 0, 0, 255);
            SDL_RenderDrawLine(rend1, 0, SCREEN_HEIGHT - 40 * i, SCREEN_WIDTH, SCREEN_HEIGHT - 40 * i);
        }

        // 10 Lineas verticales
        for (int i = 1; i < 11; i++) {
            SDL_SetRenderDrawColor(rend1, 0, 0, 0, 255);
            SDL_RenderDrawLine(rend1, SCREEN_WIDTH - 48 * i, 0, SCREEN_WIDTH - 48 * i, SCREEN_HEIGHT);
        }
        //SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
        SDL_RenderClear(rend1);
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderPresent(rend);

        //calcula el juego a 60fps
        SDL_Delay(1000 / 60);
    }
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}