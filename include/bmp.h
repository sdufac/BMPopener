#ifndef BMP_H
#define BMP_H
#include <stdio.h>
#include <stdint.h>
#include "raylib.h"

bool isFileBMP(FILE* image);
uint32_t getImageWidth(FILE* image);
uint32_t getImageHeight(FILE* image);
uint16_t getColorBitCount(FILE *image);
uint32_t getColorUsed(FILE *image);
uint32_t getDataOffset(FILE* image);
uint32_t getPixelDataSize(FILE* image);
unsigned char* getPixelData(FILE *image, uint32_t dataOffset, int pixelCount, int bpp, int width, int height);
uint32_t getCompression(FILE *image);

#endif
