#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include "../include/bmp.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
	char* fileName = argv[1];
	FILE *image = fopen(fileName,"rb");
	if(!image){
		perror("Erreur lors de l'ouverture du fichier");
	}

	bool isBMP = isFileBMP(image);
	int screenWidth;
	int screenHeight;

	if(isBMP){
		screenWidth = getImageWidth(image);
		screenHeight = getImageHeight(image);
	}else{
		screenWidth = 500;
		screenHeight = 500;
	}

	int pixelCount = screenWidth * screenHeight;
	uint16_t colourBitCount = getColorBitCount(image);
	uint32_t colorUsed = getColorUsed(image);
	uint32_t dataOffset = getDataOffset(image);
	uint32_t compresion = getCompression(image);
	uint32_t dataSize = getPixelDataSize(image);
	printf("Taille de raster data = %d\n",dataSize);
	unsigned char *pixelData = NULL;
	int bpp = colourBitCount / 8;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer("BMPopener", screenWidth, screenHeight, 0, &window, &renderer);
	SDL_Surface *surface = NULL;

	switch(colourBitCount){
		case 1:
			break;
		case 4:
			break;
		case 8:
			break;
		case 16:
			pixelData = getPixelData(image, dataOffset, pixelCount, bpp,screenWidth,screenHeight);
			surface = SDL_CreateSurfaceFrom(screenWidth, screenHeight, SDL_PIXELFORMAT_RGB565, pixelData, screenWidth * bpp);
			break;
		case 24:
			pixelData = getPixelData(image, dataOffset, pixelCount, bpp,screenWidth,screenHeight);
			surface = SDL_CreateSurfaceFrom(screenWidth, screenHeight, SDL_PIXELFORMAT_BGR24, pixelData, screenWidth * bpp);
			break;
		case 32:
			break;
		default:
			break;
	}

	if(!surface){
		SDL_Log("Impossible de creer la surface: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if(!texture){
		SDL_Log("Impossible de creer la texture: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
	if(event->type == SDL_EVENT_QUIT){
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate){
	SDL_FRect rect;
	rect.x = 0;
	rect.y = 0;
	rect.h = texture->h;
	rect.w = texture->w;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	SDL_RenderTexture(renderer, texture, NULL, &rect);
	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){

}
