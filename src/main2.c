#include "raylib.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include "../include/bmp.h"
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main(int argc,char * argv[]){
	// Initialization
	//--------------------------------------------------------------------------------------
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
	Image img;

	switch(colourBitCount){
		case 1:
			break;
		case 4:
			break;
		case 8:
			break;
		case 16:
			break;
		case 24:
			img = getImage24bit(image,dataOffset,pixelCount,screenWidth, screenHeight);
			break;
		case 32:
			break;
		default:
			break;
	}

	if(img.data != NULL){
		Texture2D texture = LoadTextureFromImage(img);
	}

	InitWindow(screenWidth, screenHeight, "BMP viewer");

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

			// ClearBackground(RAYWHITE);
			// DrawTexture(texture,screenWidth/2,screenHeight/2,WHITE); 

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
