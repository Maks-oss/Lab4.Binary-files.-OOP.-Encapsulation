#include<fstream>
#include <iostream>

using namespace std;

typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;
typedef struct
{
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct
{
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
} RGB;



int main()
{
	FILE* f = fopen("bmp.bmp", "r");
	FILE* test = fopen("d:\\new.bmp", "w");
	
	BITMAPFILEHEADER bmp_h;
	BITMAPINFOHEADER bmp_i;
	fread(&bmp_h, sizeof(BITMAPFILEHEADER), 1, f);
	fread(&bmp_i, sizeof(BITMAPINFOHEADER), 1, f);
	//test
	return 0;
}

