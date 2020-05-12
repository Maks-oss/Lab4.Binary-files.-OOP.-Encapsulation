#include <fstream>
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
	BYTE rgbBlue=0;
	BYTE rgbGreen=0;
	BYTE rgbRed=0;
} RGB;
class Picture
{
private:
	BITMAPFILEHEADER bmphead;
	BITMAPINFOHEADER bmpinfo;
public:
	Picture(ifstream& file);
	void makeBigger(ifstream& file, int n, ofstream&out);
	void makeSmaller(ifstream& file, int n, ofstream& out);
};
Picture::Picture(ifstream&file)
{
	file.read((char*)&bmphead, sizeof(bmphead));
	file.read((char*)&bmpinfo, sizeof(bmpinfo));
}
void Picture::makeBigger(ifstream&file, int n, ofstream&out)
{
	int width = bmpinfo.biWidth;
	int height = bmpinfo.biHeight;
	bmpinfo.biWidth = bmpinfo.biWidth * n;
	bmpinfo.biHeight = bmpinfo.biHeight * n;
	int oldpadding = (4 - (width * sizeof(RGB)) % 4) % 4;
	int padding = (4 - (bmpinfo.biWidth * sizeof(RGB)) % 4)%4;
	bmpinfo.biSizeImage = (bmpinfo.biWidth * sizeof(RGB) + padding) * abs(bmpinfo.biHeight);
	bmphead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpinfo.biSizeImage;
	out.write((char*)&bmphead, sizeof(bmphead));
	out.write((char*)&bmpinfo, sizeof(bmpinfo));
	RGB* rgb = new RGB [width];
	for (int i = 0; i < height; i+=1)
	{
		for (int k = 0; k < width; k++) file.read((char*)&rgb[k], sizeof(RGB));
		file.seekg(oldpadding,file.cur);
		for (int l = 0; l < n; l++)
		{
			for (int kk = 0; kk < width; kk++)
			{
				for (int ll = 0; ll < n; ll++)
				{
					out.write((char*)&rgb[kk], sizeof(RGB));
				}
			}
			for (int p = 0; p < padding; p++) out.put(0x00);
		}
	}
}
void Picture::makeSmaller(ifstream& file, int n, ofstream& out)
{
	int width = bmpinfo.biWidth;
	int height = bmpinfo.biHeight;
	bmpinfo.biWidth = bmpinfo.biWidth/n;
	bmpinfo.biHeight = bmpinfo.biHeight/n;
	int oldpadding = (4 - (width * sizeof(RGB)) % 4) % 4;
	int padding = (4 - (bmpinfo.biWidth * sizeof(RGB)) % 4) % 4;
	bmpinfo.biSizeImage = (bmpinfo.biWidth * sizeof(RGB) + padding) * abs(bmpinfo.biHeight);
	bmphead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpinfo.biSizeImage;
	out.write((char*)&bmphead, sizeof(bmphead));
	out.write((char*)&bmpinfo, sizeof(bmpinfo));
	RGB** rgb = new RGB* [height];
	for (int i = 0; i < height; i++) rgb[i] = new RGB[width];
	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			file.read((char*)&rgb[i][j], sizeof(RGB));
		}
		file.seekg(oldpadding, file.cur);
	}
	RGB** newrgb = new RGB* [bmpinfo.biHeight];
	for (int i = 0; i < bmpinfo.biHeight; i++) newrgb[i] = new RGB[bmpinfo.biWidth];
	for (int i = 0; i < height-height%n; i++)
	{
		for (int j = 0; j < width-width%n; j++)
		{
			newrgb[i/n][j/n].rgbBlue = rgb[i-(i%n)/2][j-(j%n)/2].rgbBlue;
			newrgb[i/n][j/n].rgbGreen = rgb[i-(i%n)/2][j-(j%n)/2].rgbGreen;
			newrgb[i/n][j/n].rgbRed = rgb[i-(i%n)/2][j-(j%n)/2].rgbRed;
		}
	}
	for (int i = 0; i < bmpinfo.biHeight; i++)
	{
		for (int j = 0; j < bmpinfo.biWidth; j++)
		{
			out.write((char*)&newrgb[i][j], sizeof(RGB));
		}
		for (int p = 0; p < padding; p++)
		{
			out.put(0);
		}
	}
}
int main(int argc, char*argv[])
{
	string inputfile=argv[1];
	string outputfile=argv[2];
	double n = atof(argv[3]);///
	ifstream file(inputfile, ios::binary);
	if (n == int(n))
	{
		ofstream out(outputfile, ios::binary);
		Picture BMP(file);
		BMP.makeBigger(file, n, out);
		file.close();
		out.close();
		printf("\nBOLSHE!");
		return 1;
	}
	else if (n < 1)
	{
		ofstream out("help.bmp", ios::binary);
		Picture BMP(file);
		int nn = round(n * 10);
		BMP.makeBigger(file, nn, out);
		file.close();
		out.close();
		ifstream tfile("help.bmp", ios::binary);
		ofstream tout(outputfile, ios::binary);
		Picture tBMP(tfile);
		tBMP.makeSmaller(tfile, 10, tout);
		tfile.close();
		tout.close();
		printf("\nMENSHE");
		return 2;
	}
	else 
	{
		ofstream out("help.bmp", ios::binary);
		Picture BMP(file);
		int ni = int(n)+1;
		n = n/ni;
		int nn = round(n * 10);
		BMP.makeBigger(file, ni*nn, out);
		file.close();
		out.close();
		ifstream tfile("help.bmp", ios::binary);
		ofstream tout(outputfile, ios::binary);
		Picture tBMP(tfile);
		tBMP.makeSmaller(tfile, 10, tout);
		tfile.close();
		tout.close();
		printf("BOLSHE(no ne celoe)");
		return 3;
	}
	return -1;
}