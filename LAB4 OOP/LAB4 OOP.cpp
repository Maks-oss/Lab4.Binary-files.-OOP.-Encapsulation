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
	BYTE rgbBlue=0x00;
	BYTE rgbGreen=0x00;
	BYTE rgbRed=0x00;
} RGB;

class Picture
{
private:
	BITMAPFILEHEADER bmphead;
	BITMAPINFOHEADER bmpinfo;
public:
	Picture(ifstream&file);
	void makeBigger(ifstream&file,int n,ofstream&out);
	void simpleout(ifstream& file, ofstream& out);
	void inc(ifstream& file, int n, ofstream& out);
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
	for (int i = 0; i <= height; i+=1)
	{
		for (int k = 0; k <= width; k+=1)
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

void Picture::simpleout(ifstream& file, ofstream& out)
{
	int width = bmpinfo.biWidth;
	int height = bmpinfo.biHeight;
	int oldpadding = (4 - (width * sizeof(RGB)) % 4) % 4;
	int padding = (4 - (bmpinfo.biWidth * sizeof(RGB)) % 4) % 4;

	out.write((char*)&bmphead, sizeof(bmphead));
	out.write((char*)&bmpinfo, sizeof(bmpinfo));

	RGB** rgb = new RGB * [height+1];
	for (int i = 0; i <= height; i += 1) rgb[i] = new RGB[width+1];

	for (int i = 0; i <= height; i += 1)
	{
		for (int j = 0; j <= width; j += 1)
		{
			file.read((char*)&rgb[i][j], sizeof(RGB));
		}
	}
	for (int i = 0; i <= height; i += 1)
	{
		for (int j = 0; j <= width; j += 1)
		{
			out.write((char*)&rgb[i][j], sizeof(RGB));
		}
	}
}
int main()
{
	ifstream file("d:\\bmpp.bmp", ios::binary);
	ofstream out("d:\\HAHA.bmp", ios::binary);
	Picture BMP(file);

	BMP.simpleout(file, out);
	//BMP.inc(file, 4, out);

	return 0;
}

