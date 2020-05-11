#include<fstream>
#include <iostream>

#pragma pack(1)

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
	Picture(ifstream& file);
	void makeBigger(ifstream& file, int n, ofstream&out);
};

Picture::Picture(ifstream&file)
{
	file.read((char*)&bmphead, sizeof(bmphead));
	file.read((char*)&bmpinfo, sizeof(bmpinfo));
}

void Picture::makeBigger(ifstream&file,int n, ofstream&out)
{
	int width = bmpinfo.biWidth;
	int height = bmpinfo.biHeight;
	//
	bmpinfo.biWidth = bmpinfo.biWidth * n;
	bmpinfo.biHeight = bmpinfo.biHeight * n;
	//int oldpadding = (4 - (width * sizeof(RGB)) % 4) % 4;
	int padding = (4 - (bmpinfo.biWidth * sizeof(RGB)) % 4)%4;
	bmpinfo.biSizeImage = ((bmpinfo.biWidth + padding) * sizeof(RGB)) + bmpinfo.biHeight;
	bmphead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpinfo.biSizeImage;
	//
	out.write((char*)&bmphead, sizeof(bmphead));
	out.write((char*)&bmpinfo, sizeof(bmpinfo));

	RGB* rgb = new RGB [width];

	cout << width << " " << height << "\n";
	cout << bmpinfo.biWidth << " " << bmpinfo.biHeight << "\n";

	for (int i = 0; i < height; i+=1)
	{
		for (int k = 0; k < width; k+=1)
		{
			file.read((char*)&rgb[k], sizeof(RGB));
		}
		for (int l = 0; l < n; l++)
		{
			int kk;
			for (kk = 0; kk < width; kk += 1)
			{
				for (int ll = 0; ll < n; ll++)
				{
					out.write((char*)&rgb[kk], sizeof(RGB));
				}
				for (int p = 0; p < padding; p++)
				{
					out.write("0x00", sizeof(RGB));
				}
			}
		}
	}
}

int main()
{
	ifstream file("d:\\bmpp.bmp", ios::binary);
	ofstream out("d:\\HAHA.bmp", ios::binary);
	Picture BMP(file);

	//BMP.simpleout(file, out);
	BMP.makeBigger(file, 4, out);

	return 0;
}

