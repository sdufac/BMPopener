#include "../include/bmp.h"
#include <string.h>
#include <stdlib.h>

bool isFileBMP(FILE* image){

	uint16_t sgntr = 0x4D42;
	uint16_t extractedSgntr = 0;
	char buffer[2];

	fread(buffer,2,1,image);
	if(ferror(image)){
		perror("Erreur lors de la lecture de la signature");
	}
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
	if(ferror(image)){
		perror("Erreur lors de la lecture de la largeur");
	}
	memcpy(&width,buffer,sizeof(buffer));

	printf("Largeur de l'image %d\n",width);
	return width;
}

uint32_t getImageHeight(FILE* image){
	char buffer[4];
	
	uint32_t height = 0;

	fseek(image,22,SEEK_SET);
	fread(buffer, 4, 1 ,image);
	if(ferror(image)){
		perror("Erreur lors de la lecture de la hauteur de l'image");
	}
	memcpy(&height,buffer,sizeof(buffer));

	printf("Hauteur de l'image %d\n",height);
	return height;
}

uint16_t getColorBitCount(FILE *image){
	uint16_t bitCount;
	char bufferBitCount[2];

	fseek(image,28,SEEK_SET);
	fread(bufferBitCount,2,1,image);
	if(ferror(image)){
		perror("Erreur lors de la lecture du BitCount");
	}

	memcpy(&bitCount,bufferBitCount,2);
	printf("Color bit count = %d\n",bitCount);

	return bitCount;
}

uint32_t getColorUsed(FILE *image){
	uint32_t colorUsed;
	char bufferColorUsed[4];

	fseek(image,46,SEEK_SET);
	fread(bufferColorUsed,4,1,image);
	if(ferror(image)){
		perror("Erreur lors de la lecture de ColorUsed");
	};

	memcpy(&colorUsed,bufferColorUsed,4);
	printf("Number of color used = %d\n",colorUsed);

	return colorUsed;
}

uint32_t getDataOffset(FILE* image){
	uint32_t dataOffset;
	char bufferDataOffset[4];

	fseek(image,10,SEEK_SET);
	fread(bufferDataOffset,4,1,image);

	if(ferror(image)){
		perror("Erreur lors de la lecture de DataOffset");
	}

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
	if(fread(img.data,sizeof(char),dataLength,image)< dataLength || ferror(image)){
		if(feof(image))printf("Fin du fichier atteint\n");
		perror("Erreur lors de la lecture des données des pixels");
	}

	return img;
}
