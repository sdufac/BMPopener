#include "raylib.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

typedef struct{
	Color color;
	int posX;
	int posY;
}Pixel;

bool isFileBMP(FILE* image);
uint32_t getImageWidth(FILE* image);
uint32_t getImageHeight(FILE* image);
uint16_t getColorBitCount(FILE *image);
uint32_t getColorUsed(FILE *image);
uint32_t getDataOffset(FILE* image);
Image getImage24bit(FILE *image, uint32_t dataOffset, int pixelCount, int imageWidth, int imageHeight);

int main(int argc,char * argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	char* fileName = argv[1];
	FILE *image = fopen(fileName,"r");
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
	Texture2D texture = LoadTextureFromImage(img);

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

bool isFileBMP(FILE* image){

	uint16_t sgntr = 0x4D42;
	uint16_t extractedSgntr = 0;
	char buffer[2];

	fread(buffer,2,1,image);
	memcpy(&extractedSgntr,buffer,2);
	
	if(sgntr == extractedSgntr){
		return true;
	}else{
		return false;
	}
}

uint32_t getImageWidth(FILE* image){
	char buffer[4];
	
	uint32_t width = 0;

	fseek(image,18,SEEK_SET);
	fread(buffer, 4, 1 ,image);
	memcpy(&width,buffer,sizeof(buffer));

	printf("Largeur de l'image %d\n",width);
	return width;
}

uint32_t getImageHeight(FILE* image){
	char buffer[4];
	
	uint32_t height = 0;

	fseek(image,22,SEEK_SET);
	fread(buffer, 4, 1 ,image);
	memcpy(&height,buffer,sizeof(buffer));

	printf("Hauteur de l'image %d\n",height);
	return height;
}

uint16_t getColorBitCount(FILE *image){
	uint16_t bitCount;
	char bufferBitCount[2];

	fseek(image,28,SEEK_SET);
	fread(bufferBitCount,2,1,image);
	memcpy(&bitCount,bufferBitCount,2);
	printf("Color bit count = %d\n",bitCount);

	return bitCount;
}

uint32_t getColorUsed(FILE *image){
	uint32_t colorUsed;
	char bufferColorUsed[4];

	fseek(image,46,SEEK_SET);
	fread(bufferColorUsed,4,1,image);
	memcpy(&colorUsed,bufferColorUsed,4);
	printf("Number of color used = %d\n",colorUsed);

	return colorUsed;
}

uint32_t getDataOffset(FILE* image){
	uint32_t dataOffset;
	char bufferDataOffset[4];

	fseek(image,10,SEEK_SET);
	fread(bufferDataOffset,4,1,image);
	memcpy(&dataOffset,bufferDataOffset,4);

	return dataOffset;
}

Image getImage24bit(FILE *image, uint32_t dataOffset, int pixelCount, int imageWidth, int imageHeight){
	int dataLength = pixelCount*3;

	Image img;
	img.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8;
	img.data = (unsigned char*)malloc(dataLength);
	img.width = imageWidth;
	img.height = imageHeight;
	img.mipmaps = 1;

	fseek(image,dataOffset,SEEK_SET);
	fread(img.data,dataLength,1,image);

	return img;
}
