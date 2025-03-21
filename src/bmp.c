#include "../include/bmp.h"
#include <stdint.h>
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

uint32_t getPixelDataSize(FILE* image){
	uint32_t dataSize;
	char buffer[4];

	fseek(image,34,SEEK_SET);
	fread(buffer,4,1,image);

	if(ferror(image)){
		perror("Erreur lors de la lecture de PixelDataSize");
	}

	memcpy(&dataSize,buffer,4);

	return dataSize;
}

unsigned char* getPixelData(FILE *image, uint32_t dataOffset, int pixelCount, int bpp, int width, int height){
	int dataLength = pixelCount*bpp;

	int bytePerLine = (width * bpp);
	int padding = 4 - ((width * bpp)%4);
	printf("Padding = %d\n",padding);

	unsigned char* buffer = (unsigned char*)malloc(dataLength);
	unsigned char* data = (unsigned char*)malloc(dataLength);

	fseek(image,dataOffset,SEEK_SET);
	if(padding == 4){
		if(fread(buffer,sizeof(char),dataLength,image)< dataLength || ferror(image)){
			if(feof(image))printf("Fin du fichier atteint\n");
			perror("Erreur lors de la lecture des données des pixels");
		}
	}else{
		for(int i = 0; i< height; i++){
			if(fread(buffer + (i * (bytePerLine - padding)),sizeof(char),bytePerLine - padding,image)< bytePerLine - padding || ferror(image)){
				if(feof(image))printf("Fin du fichier atteint\n");
				perror("Erreur lors de la lecture des données des pixels");
			}
			fseek(image, 3,SEEK_CUR);
		}
	}
	
	//Retourner l'image horizontalement
	for(int i = dataLength; i >= bytePerLine; i-= bytePerLine){
		int foo = i - bytePerLine;
		memcpy(data + (dataLength - i), buffer + foo, bytePerLine);
	}

	return data;
}

unsigned char* getPixelData4(FILE *image, uint32_t dataOffset, int pixelCount, int width, int height){
	size_t dataLength = pixelCount / 2;
	if(pixelCount%2 == 1)dataLength ++;

	unsigned char* buffer = malloc(dataLength);
	unsigned char* indexData = malloc(pixelCount);
	memset(indexData,0, pixelCount);


	if(fread(buffer,sizeof(char),dataLength,image)< dataLength || ferror(image)){
		if(feof(image))printf("Fin du fichier atteint\n");
		perror("Erreur lors de la lecture des données des pixels");
	}

	int foo = 0;
	for(int i= 0; i<dataLength; i++){
		indexData[foo++] = buffer[i] >> 4;
		if(foo < pixelCount -1){
			indexData[foo++] = buffer[i] & 0xF;
		}else{
			break;
		}
	}
	
	return indexData;
}

uint32_t getCompression(FILE *image){
	uint32_t compression;
	unsigned char* buffer[4];

	fseek(image,30,SEEK_SET);
	if(fread(buffer,4,1,image) !=1 || ferror(image)){
		if(feof(image))printf("Fin du fichier atteint\n");
		perror("Erreur lors de la lecture de la compression");
	}
	memcpy(&compression,buffer,4);

	printf("Compression = %d\n",compression);

	return compression;
}

unsigned char* getColorData(FILE *image, uint32_t colorUsed, unsigned char* pixelIndex, int nbOfPixel){
	int sizeofTable = colorUsed * 4;
	uint8_t* indexBuffer = (uint8_t*)malloc(nbOfPixel);
	uint8_t* colorBuffer = (uint8_t*)malloc(colorUsed * 4);
	unsigned char* pixelData = (unsigned char*)malloc(nbOfPixel * 3);


	memcpy(indexBuffer,pixelIndex,nbOfPixel);

	fseek(image, 54, SEEK_SET);
	if(fread(colorBuffer,4,colorUsed,image) < colorUsed || ferror(image)){
		if(feof(image))printf("Fin du fichier atteint\n");
		perror("Erreur lors de la lecture de la color table");
	}

	for(int i = 0; i<nbOfPixel; i++){
		int pixelColorIndex = indexBuffer[i];
		int colorPos;

		for(int j = 0; j < colorUsed; j++){
			int colorIndex = colorBuffer[j*4];
			if(pixelColorIndex == colorIndex){
				colorPos = j*4;
			}
		}

		memcpy(pixelData + i*3,colorBuffer + colorPos,3);
	}

	free(colorBuffer);
	free(indexBuffer);

	return pixelData;
}
