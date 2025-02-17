#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h> // 添加 bool 类型支持
#include <stdio.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void cleanup(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture) {
  if (texture)
    SDL_DestroyTexture(texture);
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("用法: %s <图片路径>\n", argv[0]);
    return 1;
  }

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL 初始化失败: %s\n", SDL_GetError());
    return 1;
  }
  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
    printf("SDL_image 初始化失败: %s\n", IMG_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("图片查看器", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                        WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (!window) {
    printf("窗口创建失败: %s\n", SDL_GetError());
    cleanup(NULL, NULL, NULL);
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    printf("渲染器创建失败: %s\n", SDL_GetError());
    cleanup(window, NULL, NULL);
    return 1;
  }

  SDL_Texture *texture =
      IMG_LoadTexture(renderer, argv[1]); // 这里假设图片路径作为命令行参数传入
  if (!texture) {
    printf("加载图片失败: %s\n", IMG_GetError());
    cleanup(window, renderer, NULL);
    return 1;
  }

  SDL_Event event;
  int running = 1;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ||
          (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
        running = 0;
      }
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  cleanup(window, renderer, texture);
  return 0;
}
