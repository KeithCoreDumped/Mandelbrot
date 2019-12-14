// Mandelbrot.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <thread>
//#include <gdiplus.h>
#include <string>
#include <windows.h>
//#include <gmpxx.h>
//#include <D:\source\repos\includes\svpng.hpp>
using namespace std;
//using namespace Gdiplus;

#define Ms


#ifdef Ms
#define xi -2.2
#define xa .9
#define yi -1.2
#define ya 1.2
#endif // Ms
#ifdef Js
#define xi -2.0
#define xa 2.0
#define yi -2.0
#define ya 2.0
#endif // Js


#define N 5000
#define W (long)((xa-(xi))*N)
#define H (long)((ya-(yi))*N)
#define MAX 25
#define J_Cx -.6
#define J_Cy .6
volatile unsigned o = 0, x = 0, y = 0;
long prg;
void output()
{
	long long st0 = 0, st1 = 0, spd = 1, a = 0;
	long long wh = W * H;
	while (1)
	{
		if (o == 1)
		{
			a++;
			st1 = y * W + x; //avg speed = %d pps\t
			spd += 2 * (st1 - st0);
			printf("%c[2K", 27);
			//printf("pixel = %d / %d\tspeed = %d PPs\tETA:%d s\n", st1, W * H /*2 * (st1 - st0), spd / a*/, (W * H - st1) / (2 * (st1 - st0)));
			cout << "pixel = " << st1 << " / " << wh << "\tspeed = " << 2 * (st1 - st0) << " PPs\tETA:" << (wh - st1) / (2 * (st1 - st0)) << " s\n";
			printf("[");
			float j = (float)st1 / W / H * 100;
			for (int i = 0; i <= 50; i++)
			{
				if (i < 24 || i>27)printf(j / 2 >= i ? "=" : " ");
				if (i == 24)printf("%.1f%%", j);
			}
			printf("]\n");
		}
		if (o == 2)
		{
			o = 3;
			st1 = 0, st0 = 0;
		}
		if (o == 3)
		{
			st1 = prg;
			printf("%.2f/%.2f MB \t speed=%.2f MB/s\n", st1 / 1048576.0, (float)W * H * 3 / 1048576, (st1 - st0) / 524288.0);
		}
		if (o == 1)st0 = y * W + x;
		if (o == 3)st0 = prg;
		Sleep(500);

	}
}
void svpng(FILE* fp, unsigned w, unsigned h, const unsigned char* img, int alpha, long* prg);
unsigned ptm(double x, double y)//mandelbrot
{
	double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	unsigned i = 0;
	for (; i <= MAX && x1 * x1 + y1 * y1 < 4; i++)
	{//循环展开
		x2 = x1, y2 = y1;
		x1 = x2 * x2 - y2 * y2 + x, y1 = 2 * x2 * y2 + y;
		x2 = x1, y2 = y1;
		x1 = x2 * x2 - y2 * y2 + x, y1 = 2 * x2 * y2 + y;
		x2 = x1, y2 = y1;
		x1 = x2 * x2 - y2 * y2 + x, y1 = 2 * x2 * y2 + y;
		x2 = x1, y2 = y1;
		x1 = x2 * x2 - y2 * y2 + x, y1 = 2 * x2 * y2 + y;
	}

	return i == MAX ? 0 : i;
}
unsigned ptj(double x, double y)
{
	double x2 = 0, y2 = 0;
	unsigned i = 0;
	for (; i <= MAX && x * x + y * y < 4; i++)
	{
		x2 = x, y2 = y;
		x = x2 * x2 - y2 * y2 + J_Cx, y = 2 * x2 * y2 + J_Cy;
	}
	return i == MAX ? 0 : i;
}

int main(int argc, char* argv[])
{
	for (int a = 0; a < argc; a++)
	{
		cout << a << " - " << argv[a] << endl;
		if (!strcmp(argv[a], "--help"))
			cout << "M -Xmin <double> -Xmax <double> -Ymin <double> -Ymax <double> -s <double> -o <string>";

	}
	system("pause");
	BYTE* img = new BYTE[W * H * 3];
	if (!img)
	{
		cout << "emmmm";
		system("pause");
	}
	BYTE* p = img;
	thread t(output);
	t.detach();
	long i = 0, imax = 0;
	long double w = 0, h = 0, zx = 0, zy = 0, zx1 = 0, zy1 = 0;
	o = 1;
	for (y = 0; y < H; y++)
	{
		for (x = 0; x < W; x++)
		{
			w = x / (long double)W * (xa - xi) + xi;
			h = y / (long double)H * (ya - yi) + yi;
			i = ptm(w, h);
			*p++ = (double)i / MAX * 255;
			*p++ = (double)i / MAX * 255;
			*p++ = (double)i / MAX * 255;

		}
	}
	o = 0;
	FILE* fp;
	errno_t a = fopen_s(&fp, "D:\\MSet\\0.png", "wb+");
	o = 2;
	svpng(fp, W, H, img, 0, &prg);
	/*for (i = 0; i < 8; i++) fputc(("\x89PNG\r\n\32\n")[i], fp);;
	do { do { fputc((13) >> 24, fp); fputc(((13) >> 16) & 255, fp); fputc(((13) >> 8) & 255, fp); fputc((13) & 255, fp); } while (0); c = ~0U; for (i = 0; i < 4; i++) do { fputc(("IHDR")[i], fp); c ^= (("IHDR")[i]); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); } while (0);
	do { do { fputc((W) >> 24, fp); c ^= ((W) >> 24); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); do { fputc(((W) >> 16) & 255, fp); c ^= (((W) >> 16) & 255); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); do { fputc(((W) >> 8) & 255, fp); c ^= (((W) >> 8) & 255); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); do { fputc((W) & 255, fp); c ^= ((W) & 255); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); } while (0); do { do { fputc((h) >> 24, fp); c ^= ((h) >> 24); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); do { fputc(((h) >> 16) & 255, fp); c ^= (((h) >> 16) & 255); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); do { fputc(((h) >> 8) & 255, fp); c ^= (((h) >> 8) & 255); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); do { fputc((h) & 255, fp); c ^= ((h) & 255); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); } while (0);
	do { fputc(8, fp); c ^= (8); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); do { fputc(alpha ? 6 : 2, fp); c ^= (alpha ? 6 : 2); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0);
	for (i = 0; i < 3; i++) do { fputc(("\0\0\0")[i], fp); c ^= (("\0\0\0")[i]); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0);
	do { fputc((~c) >> 24, fp); fputc(((~c) >> 16) & 255, fp); fputc(((~c) >> 8) & 255, fp); fputc((~c) & 255, fp); } while (0);
	do { do { fputc((2 + h * (5 + p) + 4) >> 24, fp); fputc(((2 + h * (5 + p) + 4) >> 16) & 255, fp); fputc(((2 + h * (5 + p) + 4) >> 8) & 255, fp); fputc((2 + h * (5 + p) + 4) & 255, fp); } while (0); c = ~0U; for (i = 0; i < 4; i++) do { fputc(("IDAT")[i], fp); c ^= (("IDAT")[i]); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); } while (0);
	for (i = 0; i < 2; i++) do { fputc(("\x78\1")[i], fp); c ^= (("\x78\1")[i]); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0);
	for (y = 0; y < h; y++) {
		do { fputc(y == h - 1, fp); c ^= (y == h - 1); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0);
		do { do { fputc((p) & 255, fp); c ^= ((p) & 255); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); do { fputc(((p) >> 8) & 255, fp); c ^= (((p) >> 8) & 255); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); } while (0); do { do { fputc((~p) & 255, fp); c ^= ((~p) & 255); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); do { fputc(((~p) >> 8) & 255, fp); c ^= (((~p) >> 8) & 255); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); } while (0);
		do { do { fputc(0, fp); c ^= (0); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); a = (a + (0)) % 65521; b = (b + a) % 65521; } while (0);
		for (x = 0; x < p - 1; x++, img++) {
			do { do { fputc(*img, fp); c ^= (*img); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); a = (a + (*img)) % 65521; b = (b + a) % 65521; } while (0);
			(*prg)++;
		}
	}
	do { do { fputc(((b << 16) | a) >> 24, fp); c ^= (((b << 16) | a) >> 24); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); do { fputc((((b << 16) | a) >> 16) & 255, fp); c ^= ((((b << 16) | a) >> 16) & 255); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); do { fputc((((b << 16) | a) >> 8) & 255, fp); c ^= ((((b << 16) | a) >> 8) & 255); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); do { fputc(((b << 16) | a) & 255, fp); c ^= (((b << 16) | a) & 255); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); } while (0);
	do { fputc((~c) >> 24, fp); fputc(((~c) >> 16) & 255, fp); fputc(((~c) >> 8) & 255, fp); fputc((~c) & 255, fp); } while (0);
	do { do { fputc((0) >> 24, fp); fputc(((0) >> 16) & 255, fp); fputc(((0) >> 8) & 255, fp); fputc((0) & 255, fp); } while (0); c = ~0U; for (i = 0; i < 4; i++) do { fputc(("IEND")[i], fp); c ^= (("IEND")[i]); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while (0); } while (0); do { fputc((~c) >> 24, fp); fputc(((~c) >> 16) & 255, fp); fputc(((~c) >> 8) & 255, fp); fputc((~c) & 255, fp); } while (0);
	*/

	return 0;
}

void svpng(FILE* fp, unsigned w, unsigned h, const unsigned char* img, int alpha, long* prg) {
	static const unsigned t[] = { 0, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
						  0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c, 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c };
	unsigned a = 1, b = 0, c, p = w * (alpha ? 4 : 3) + 1, x, y, i;
	for (i = 0; i < 8; i++)
		fputc(("\x89PNG\r\n\32\n")[i], fp);;
	do {
		do {
			fputc((13) >> 24, fp);
			fputc(((13) >> 16) & 255, fp);
			fputc(((13) >> 8) & 255, fp); fputc((13) & 255, fp);
		} while (0);
		c = ~0U;
		for (i = 0; i < 4; i++)
			do {
				fputc(("IHDR")[i], fp);
				c ^= (("IHDR")[i]); c = (c >> 4) ^ t[c & 15];
				c = (c >> 4) ^ t[c & 15];
			} while (0);
	} while (0);
	do {
		do {
			fputc((W) >> 24, fp);
			c ^= ((W) >> 24);
			c = (c >> 4) ^ t[c & 15];
			c = (c >> 4) ^ t[c & 15];
		} while (0);
		do {
			fputc(((W) >> 16) & 255, fp);
			c ^= (((W) >> 16) & 255);
			c = (c >> 4) ^ t[c & 15];
			c = (c >> 4) ^ t[c & 15];
		} while (0);
		do {
			fputc(((W) >> 8) & 255, fp);
			c ^= (((W) >> 8) & 255);
			c = (c >> 4) ^ t[c & 15];
			c = (c >> 4) ^ t[c & 15];
		} while (0);
		do {
			fputc((W) & 255, fp);
			c ^= ((W) & 255);
			c = (c >> 4) ^ t[c & 15];
			c = (c >> 4) ^ t[c & 15];
		} while (0);
	} while (0);
	do {
		do {
			fputc((h) >> 24, fp);
			c ^= ((h) >> 24);
			c = (c >> 4) ^ t[c & 15];
			c = (c >> 4) ^ t[c & 15];
		} while (0);
		do {
			fputc(((h) >> 16) & 255, fp);
			c ^= (((h) >> 16) & 255);
			c = (c >> 4) ^ t[c & 15];
			c = (c >> 4) ^ t[c & 15];
		} while (0);
		do {
			fputc(((h) >> 8) & 255, fp);
			c ^= (((h) >> 8) & 255);
			c = (c >> 4) ^ t[c & 15];
			c = (c >> 4) ^ t[c & 15];
		} while (0);
		do {
			fputc((h) & 255, fp);
			c ^= ((h) & 255);
			c = (c >> 4) ^ t[c & 15];
			c = (c >> 4) ^ t[c & 15];
		} while (0);
	} while (0);
	do {
		fputc(8, fp);
		c ^= (8);
		c = (c >> 4) ^ t[c & 15];
		c = (c >> 4) ^ t[c & 15];
	} while (0);
	do {
		fputc(alpha ? 6 : 2, fp);
		c ^= (alpha ? 6 : 2);
		c = (c >> 4) ^ t[c & 15];
		c = (c >> 4) ^ t[c & 15];
	} while (0);
	for (i = 0; i < 3; i++)
		do {
			fputc(("\0\0\0")[i], fp);
			c ^= (("\0\0\0")[i]);
			c = (c >> 4) ^ t[c & 15];
			c = (c >> 4) ^ t[c & 15];
		} while (0);
		do {
			fputc((~c) >> 24, fp);
			fputc(((~c) >> 16) & 255, fp);
			fputc(((~c) >> 8) & 255, fp);
			fputc((~c) & 255, fp);
		} while (0);
		do {
			do
			{
				fputc((2 + h * (5 + p) + 4) >> 24, fp);
				fputc(((2 + h * (5 + p) + 4) >> 16) & 255, fp); fputc(((2 + h * (5 + p) + 4) >> 8) & 255, fp); fputc((2 + h * (5 + p) + 4) & 255, fp);
			} while (0);
			c = ~0U;
			for (i = 0; i < 4; i++)
				do {
					fputc(("IDAT")[i], fp);
					c ^= (("IDAT")[i]);
					c = (c >> 4) ^ t[c & 15];
					c = (c >> 4) ^ t[c & 15];
				} while (0);
		} while (0);
		for (i = 0; i < 2; i++)
			do {
				fputc(("\x78\1")[i], fp);
				c ^= (("\x78\1")[i]);
				c = (c >> 4) ^ t[c & 15];
				c = (c >> 4) ^ t[c & 15];
			} while (0);
			for (y = 0; y < h; y++)
			{
				do {
					fputc(y == h - 1, fp);
					c ^= (y == h - 1);
					c = (c >> 4) ^ t[c & 15];
					c = (c >> 4) ^ t[c & 15];
				} while (0);
				do {
					do
					{
						fputc((p) & 255, fp);
						c ^= ((p) & 255);
						c = (c >> 4) ^ t[c & 15];
						c = (c >> 4) ^ t[c & 15];
					} while (0);
					do {
						fputc(((p) >> 8) & 255, fp);
						c ^= (((p) >> 8) & 255);
						c = (c >> 4) ^ t[c & 15];
						c = (c >> 4) ^ t[c & 15];
					} while (0);
				} while (0);
				do {
					do {
						fputc((~p) & 255, fp);
						c ^= ((~p) & 255);
						c = (c >> 4) ^ t[c & 15];
						c = (c >> 4) ^ t[c & 15];
					} while (0);
					do {
						fputc(((~p) >> 8) & 255, fp);
						c ^= (((~p) >> 8) & 255);
						c = (c >> 4) ^ t[c & 15];
						c = (c >> 4) ^ t[c & 15];
					} while (0);
				} while (0);
				do {
					do {
						fputc(0, fp);
						c ^= (0);
						c = (c >> 4) ^ t[c & 15];
						c = (c >> 4) ^ t[c & 15];
					} while (0);
					a = (a + (0)) % 65521;
					b = (b + a) % 65521;
				} while (0);

				//for (x = 0; x < p - 1; x++, img++)
				for (x = 1; x < p; x++, img++)
				{
					do {
						do {
							fputc(*img, fp);
							c ^= (*img);
							c = (c >> 4) ^ t[c & 15];
							c = (c >> 4) ^ t[c & 15];
						} while (0);
						a = (a + (*img)) % 65521;
						b = (b + a) % 65521;
					} while (0);
					(*prg)++;
				}
			}
			do {
				do {
					fputc(((b << 16) | a) >> 24, fp);
					c ^= (((b << 16) | a) >> 24);
					c = (c >> 4) ^ t[c & 15];
					c = (c >> 4) ^ t[c & 15];
				} while (0);
				do {
					fputc((((b << 16) | a) >> 16) & 255, fp);
					c ^= ((((b << 16) | a) >> 16) & 255);
					c = (c >> 4) ^ t[c & 15];
					c = (c >> 4) ^ t[c & 15];
				} while (0);
				do {
					fputc((((b << 16) | a) >> 8) & 255, fp);
					c ^= ((((b << 16) | a) >> 8) & 255);
					c = (c >> 4) ^ t[c & 15];
					c = (c >> 4) ^ t[c & 15];
				} while (0);
				do {
					fputc(((b << 16) | a) & 255, fp);
					c ^= (((b << 16) | a) & 255);
					c = (c >> 4) ^ t[c & 15];
					c = (c >> 4) ^ t[c & 15];
				} while (0);
			} while (0);

			do {
				fputc((~c) >> 24, fp);
				fputc(((~c) >> 16) & 255, fp);
				fputc(((~c) >> 8) & 255, fp);
				fputc((~c) & 255, fp);
			} while (0);
			do {
				do {
					fputc((0) >> 24, fp);
					fputc(((0) >> 16) & 255, fp);
					fputc(((0) >> 8) & 255, fp);
					fputc((0) & 255, fp);
				} while (0);
				c = ~0U;
				for (i = 0;
					i < 4;
					i++) do {
					fputc(("IEND")[i], fp);
					c ^= (("IEND")[i]);
					c = (c >> 4) ^ t[c & 15];
					c = (c >> 4) ^ t[c & 15];
				} while (0);
			} while (0);
			do {
				fputc((~c) >> 24, fp);
				fputc(((~c) >> 16) & 255, fp);
				fputc(((~c) >> 8) & 255, fp);
				fputc((~c) & 255, fp);
			} while (0);
}
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
