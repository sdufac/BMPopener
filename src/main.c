#include "raylib.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
bool isFileBMP(FILE* image);
uint32_t getImageWidth(FILE* image);
uint32_t getImageHeight(FILE* image);

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

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

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

		ClearBackground(RAYWHITE);
		if(isBMP){
			DrawText("The file is BMP", 190, 200, 20, LIGHTGRAY);
		}else{
			DrawText("The file is not BMP", 190, 200, 20, LIGHTGRAY);
		}

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
