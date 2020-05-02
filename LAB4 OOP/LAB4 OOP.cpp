#include<fstream>
#include <iostream>

using namespace std;
#pragma pack(1)
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

class Picture
{
private:
	BITMAPFILEHEADER bmphead;
	BITMAPINFOHEADER bmpinfo;
public:
	Picture(ifstream&file);
	void makeBigger(ifstream&file,int n,ofstream&out);

};

Picture::Picture(ifstream&file)
{
	file.read((char*)&bmphead, sizeof(bmphead));
	file.read((char*)&bmpinfo, sizeof(bmpinfo));
}

void Picture::makeBigger(ifstream&file,int n,ofstream&out)
{
	int width = bmpinfo.biWidth;
	int height = bmpinfo.biHeight;
	//
	bmpinfo.biWidth = bmpinfo.biWidth * n;
	bmpinfo.biHeight = bmpinfo.biHeight * n;
	int oldpadding = (4 - (width * sizeof(RGB)) % 4) % 4;
	int padding = (4 - (bmpinfo.biWidth * sizeof(RGB)) % 4) % 4;
	bmpinfo.biSizeImage = (bmpinfo.biWidth * sizeof(RGB) + padding) + abs(bmpinfo.biHeight);
	bmphead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpinfo.biSizeImage;
	//
	out.write((char*)&bmphead, sizeof(bmphead));
	out.write((char*)&bmpinfo, sizeof(bmpinfo));

	for (int i = 0, biHeight = abs(bmpinfo.biHeight); i < biHeight; i++)
	{
		for (int k = 0; k < bmpinfo.biWidth; k++)
		{
			RGB triple;
			file.read((char*)&triple, sizeof(RGB));

			for (int l = 0; l < n; l++)
			{
				out.write((char*)&triple, sizeof(RGB));
			}
		}
	}
}

int main()
{
	
	ifstream file("bmp.bmp", ios::binary);
	ofstream out("d:\\HAHA.bmp", ios::binary);
	Picture BMP(file);
	BMP.makeBigger(file, 5, out);

	return 0;
}

