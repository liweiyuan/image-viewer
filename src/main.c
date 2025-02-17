#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// 初始化 SDL 和创建窗口及渲染器
bool initSDL(SDL_Window **window, SDL_Renderer **renderer);

// 加载纹理
SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path);

// 渲染纹理到窗口
void renderTexture(SDL_Renderer *renderer, SDL_Texture *texture);

// 清理和释放资源
void cleanup(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture);

int main(int argc, char *argv[]) {
  // 检查命令行参数
  if (argc < 2) {
    printf("用法: %s <图片路径>\n", argv[0]);
    return 1;
  }

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Texture *texture = NULL;

  // 初始化 SDL 和创建窗口及渲染器
  if (!initSDL(&window, &renderer)) {
    return 1;
  }

  // 加载图片纹理
  texture = loadTexture(renderer, argv[1]);
  if (!texture) {
    cleanup(window, renderer, texture);
    return 1;
  }

  // 事件循环
  SDL_Event event;
  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ||
          (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
        running = false;
      }
    }

    // 渲染纹理
    renderTexture(renderer, texture);
  }

  // 清理资源
  cleanup(window, renderer, texture);
  return 0;
}

bool initSDL(SDL_Window **window, SDL_Renderer **renderer) {
  // 初始化 SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL 初始化失败: %s\n", SDL_GetError());
    return false;
  }

  // 初始化 SDL_image
  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
    printf("SDL_image 初始化失败: %s\n", IMG_GetError());
    SDL_Quit();
    return false;
  }

  // 创建窗口
  *window = SDL_CreateWindow("图片查看器", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                             WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (!*window) {
    printf("窗口创建失败: %s\n", SDL_GetError());
    IMG_Quit();
    SDL_Quit();
    return false;
  }

  // 创建渲染器
  *renderer = SDL_CreateRenderer(
      *window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!*renderer) {
    printf("渲染器创建失败: %s\n", SDL_GetError());
    SDL_DestroyWindow(*window);
    IMG_Quit();
    SDL_Quit();
    return false;
  }

  return true;
}

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path) {
  // 加载纹理
  SDL_Texture *texture = IMG_LoadTexture(renderer, path);
  if (!texture) {
    printf("加载图片失败: %s\n", IMG_GetError());
  }
  return texture;
}

void renderTexture(SDL_Renderer *renderer, SDL_Texture *texture) {
  // 清空渲染器
  SDL_RenderClear(renderer);
  // 复制纹理到渲染器
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  // 更新渲染器
  SDL_RenderPresent(renderer);
}

void cleanup(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture) {
  // 释放纹理
  if (texture)
    SDL_DestroyTexture(texture);
  // 销毁渲染器
  if (renderer)
    SDL_DestroyRenderer(renderer);
  // 销毁窗口
  if (window)
    SDL_DestroyWindow(window);
  // 退出 SDL_image
  IMG_Quit();
  // 退出 SDL
  SDL_Quit();
}
