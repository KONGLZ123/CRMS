// sdl_demo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

extern "C"
{
#include "SDL.h"
}

int threadExit = 0;
#define REFRESH_EVENT       (SDL_USEREVENT + 1)
#define BREAK_EVENT         (SDL_USEREVENT + 2)

// �̻߳ص�����
int RefreshVideoThread(void *opaque) {
    threadExit = 0;
    while (threadExit == 0) {
        SDL_Event event;
        event.type = REFRESH_EVENT;
        SDL_PushEvent(&event);  // �����¼�
        SDL_Delay(40);
    }

    threadExit = 0;
    SDL_Event event;
    event.type = BREAK_EVENT;
    SDL_PushEvent(&event);

    return 0;
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
    }

    int screenWidth = 500;
    int screenHeight = 500;
    // ������Ƶ����
    SDL_Window *screen = SDL_CreateWindow("SDL Play", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!screen) {
        printf("SDL: could not create window - exiting: %s\n", SDL_GetError());
        return -1;
    }

    // ������Ⱦ��
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(screen, -1, 0);

    Uint32 pixFormat = SDL_PIXELFORMAT_IYUV;
    const int pixWidth = 640;
    const int pixHeight = 360;
    // ��������
    SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer, pixFormat, SDL_TEXTUREACCESS_STREAMING,
        pixWidth, pixHeight);

    FILE *fp = NULL;
    // ����Ƶ�ļ�
    fopen_s(&fp, "sintel_640_360.yuv", "rb+");
    if (NULL == fp) {
        printf("cannot open this file\n");
        return -1;
    }

    // �����߳�
    SDL_Thread *refreshThread = SDL_CreateThread(RefreshVideoThread, NULL, NULL);
    SDL_Event event;

    SDL_Rect sdlRect;
    const int bpp = 12;  // ÿ������ռ���ٱ��� YUV, 1.5 = 1 + 1/4 + 1/4 
    unsigned char buffer[pixWidth * pixHeight * bpp / 8];
    while (1) {
        SDL_WaitEvent(&event);  // �ȴ��¼�
        if (event.type == REFRESH_EVENT) {
            if (fread(buffer, 1, pixWidth * pixHeight * bpp / 8, fp) != pixWidth * pixHeight * bpp / 8) {
                fseek(fp, 0, SEEK_SET);
                fread(buffer, 1, pixWidth * pixHeight * bpp / 8, fp);
            }

            SDL_UpdateTexture(sdlTexture, NULL, buffer, pixWidth);// ������������

            sdlRect.x = 10;
            sdlRect.y = 10;
            sdlRect.w = screenWidth - 20;
            sdlRect.h = screenHeight - 20;

            SDL_RenderClear(sdlRenderer);   // �����Ⱦ��
            SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRect);    // ��������Ⱦ��
            SDL_RenderPresent(sdlRenderer); // ��ʾ��Ƶ
            // Delay 40ms
            //SDL_Delay(20);  // �����ٶ�
        }
        else if (event.type == SDL_WINDOWEVENT) {
            SDL_GetWindowSize(screen, &screenWidth, &screenHeight);
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {

        }
        else if (event.type == SDL_KEYDOWN) {

        }
        else if (event.type == SDL_QUIT) {
            threadExit = 1;
        }
        else if (event.type == BREAK_EVENT) {
            break;
        }
    }
    SDL_Quit();

    return 0;
}

