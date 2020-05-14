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
	void makeMAKE(ifstream& file, int ns, int nb, ofstream& out);
};
Picture::Picture(ifstream&file)
{
	file.read((char*)&bmphead, sizeof(bmphead));
	file.read((char*)&bmpinfo, sizeof(bmpinfo));
}
void Picture::makeMAKE(ifstream& file, int ns, int nb, ofstream& out) // ns - зменш, nb - збільш
{
	int width = bmpinfo.biWidth;
	int height = bmpinfo.biHeight;
	bmpinfo.biWidth = bmpinfo.biWidth / ns * nb;
	bmpinfo.biHeight = bmpinfo.biHeight / ns * nb;
	int oldpadding = (4 - (width * sizeof(RGB)) % 4) % 4;
	int padding = (4 - (bmpinfo.biWidth * sizeof(RGB)) % 4) % 4;
	bmpinfo.biSizeImage = (bmpinfo.biWidth * sizeof(RGB) + padding) * abs(bmpinfo.biHeight);
	bmphead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpinfo.biSizeImage;
	out.write((char*)&bmphead, sizeof(bmphead));
	out.write((char*)&bmpinfo, sizeof(bmpinfo));
	RGB** rgb = new RGB * [height];
	for (int i = 0; i < height; i++) rgb[i] = new RGB[width];
	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{
			file.read((char*)&rgb[i][j], sizeof(RGB));
		}
		file.seekg(oldpadding, file.cur);
	}
	RGB** newrgb = new RGB * [bmpinfo.biHeight];
	for (int i = 0; i < bmpinfo.biHeight; i++) newrgb[i] = new RGB[bmpinfo.biWidth];
	for (int i = 0; i < bmpinfo.biHeight; i++)
	{
		for (int j = 0; j < bmpinfo.biWidth; j++)
		{
			newrgb[i][j] = rgb[(i*ns)/nb][(j *ns)/nb];
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
int gcd(int a, int b) { return (a == 0) ? b : gcd(b % a, a); } //НОД
int main(int argc, char*argv[])
{
	string inputfile, outputfile;
	double n;
	if (argc < 3)
	{
		//cin >> inputfile >> outputfile;
		inputfile = "input.bmp", outputfile = "output.bmp";
		cin >> n;
	}
	else
	{
		inputfile=argv[1];
		outputfile=argv[2];
		n = atof(argv[3]);///
	}
	ifstream file(inputfile, ios::binary);
	ofstream out(outputfile, ios::binary);
	Picture BMP(file);
	if (n == int(n))
	{
		BMP.makeMAKE(file, 1, int(n), out);
		printf("\n CELOE");
	}
	else 
	{
		int ni = int(n)+1;
		n = n/ni;
		int nn = int(round(n*10));
		int igcd = gcd(nn*ni, 10);
		BMP.makeMAKE(file, 10/igcd, ni*nn/igcd, out);
		printf("\n NE CELOE!");
	}
	file.close();
	out.close();
	return 1;
}