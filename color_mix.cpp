#include "cimg.h"
#include "color_mix.h"
#include <cstdio>
#include <cmath>
using namespace std;
using namespace cimg_library;
//常数 
const double BLACK = 20;
const double YELLOW = 70;
/*
rgb转换为lab 
传递参数为rgb组成的三维向量 
*/
Vector3 RGB_to_LAB(Vector3 rgb)
{
	double X, Y, Z, fX, fY, fZ, L, a, b;
	double R = rgb.v[0], G = rgb.v[1], B = rgb.v[2];
    X = 0.412453 * R + 0.357580 * G + 0.180423 * B;
    Y = 0.212671 * R + 0.715160 * G + 0.072169 * B;
    Z = 0.019334 * R + 0.119193 * G + 0.950227 * B;
    X /= (255 * 0.950456);
    Y /=  255;
    Z /= (255 * 1.088754);
    if (Y > 0.008856)
	{
    	fY = pow(Y, 1.0 / 3.0);
    	L = 116.0 * fY - 16.0;
    }
	else
	{
    	fY = 7.787 * Y + 16.0 / 116.0;
    	L = 903.3 * Y;
    }
    if (X > 0.008856)
    {
    	fX = pow(X, 1.0 / 3.0);
	}
    else
    {
    	fX = 7.787*X + 16.0/116.0;
	}
    if (Z > 0.008856)
    {
    	fZ = pow(Z, 1.0 / 3.0);
	}
    else
    {
    	fZ = 7.787 * Z + 16.0 / 116.0;
	}
    a = 500.0 * (fX - fY);
    b = 200.0 * (fY - fZ);
    if (L < BLACK)
	{
    	a *= exp((L - BLACK) / (BLACK / 4));
    	b *= exp((L - BLACK) / (BLACK / 4));
    	L = BLACK;
    }
    if (b > YELLOW)
    {
     	b = YELLOW;
	}
    Vector3 res(L, a, b);
    return res;
}
/*
lab转换为rgb
传递的参数为lab组成的三维向量 
*/
Vector3 LAB_to_RGB(Vector3 lab)
{
	double X, Y, Z, fX, fY, fZ, R, G, B;
	double RR, GG, BB, L = lab.v[0], a = lab.v[1], b = lab.v[2];
    fY = pow((L + 16.0) / 116.0, 3.0);
    if (fY < 0.008856)
    {
    	fY = L / 903.3;
	}
    Y = fY;
    if (fY > 0.008856)
    {
    	fY = pow(fY, 1.0 / 3.0);
	}
    else
    {
    	fY = 7.787 * fY + 16.0/116.0;
	}
    fX = a / 500.0 + fY;      
    if (fX > 0.206893)
    {
    	X = pow(fX, 3.0);
	}
    else
    {
    	X = (fX - 16.0/116.0) / 7.787;
	}
    fZ = fY - b /200.0;      
    if (fZ > 0.206893)
    {
    	Z = pow(fZ, 3.0);
	}
    else
    {
    	Z = (fZ - 16.0/116.0) / 7.787;
	}
    X *= (0.950456 * 255);
    Y *=             255;
    Z *= (1.088754 * 255);
    RR =  3.240479 * X - 1.537150 * Y - 0.498535 * Z;
    GG = -0.969256 * X + 1.875992 * Y + 0.041556 * Z;
    BB =  0.055648 * X - 0.204043 * Y + 1.057311 * Z;
    R = (float)(RR < 0 ? 0 : RR > 255 ? 255 : RR);
    G = (float)(GG < 0 ? 0 : GG > 255 ? 255 : GG);
    B = (float)(BB < 0 ? 0 : BB > 255 ? 255 : BB);
    Vector3 res(R, G, B);
    return res;
}
double sqr(double x) 
{
	return x * x;
}
/*
输入两个文件的路径
结果保存到当前目录下
color_mix.bmp 
*/
int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("argc is not equal to 3!\n"); 
		return 0;
	} 
	CImg<double> SrcImg;
	CImg<double> TargetImg;
	SrcImg.load_bmp(argv[1]);
	TargetImg.load_bmp(argv[2]);
	int w1 = SrcImg.width();
	int h1 = SrcImg.height();
	int w2 = TargetImg.width();
	int h2 = TargetImg.height();
	CImg<double> tmpImg1(w1, h1, 1, 3, 0);
	CImg<double> tmpImg2(w2, h2, 1, 3, 0);
	CImg<double> resImg(w1, h1, 1, 3, 0);
	double meanl1 = 0.0;
	double meana1 = 0.0;
	double meanb1 = 0.0; 
	double meanl2 = 0.0;
	double meana2 = 0.0;
	double meanb2 = 0.0; 
	double sigmal1 = 0.0;
	double sigmaa1 = 0.0;
	double sigmab1 = 0.0; 
	double sigmal2 = 0.0;
	double sigmaa2 = 0.0;
	double sigmab2 = 0.0;
	cimg_forXY(tmpImg1, x, y) 
	{
		Vector3 rgb(SrcImg(x, y, 0), SrcImg(x, y, 1), SrcImg(x, y, 2));
		Vector3 lab = RGB_to_LAB(rgb);
		tmpImg1(x, y, 0) = lab.v[0]; tmpImg1(x, y, 1) = lab.v[1]; tmpImg1(x, y, 2) = lab.v[2];
		meanl1 += lab.v[0]; meana1 += lab.v[1]; meanb1 += lab.v[2];
	}
	meanl1 /= w1 * h1; meana1 /= w1 * h1; meanb1 /= w1 * h1;
	cimg_forXY(tmpImg1, x, y) 
	{
		sigmal1 += sqr(tmpImg1(x, y, 0) - meanl1);
		sigmaa1 += sqr(tmpImg1(x, y, 1) - meana1);
		sigmab1 += sqr(tmpImg1(x, y, 2) - meanb1);
	}
	sigmal1 = sqrt(sigmal1 / (double)(w1 * h1));
	sigmaa1 = sqrt(sigmaa1 / (double)(w1 * h1));
	sigmab1 = sqrt(sigmab1 / (double)(w1 * h1));
	cimg_forXY(tmpImg2, x, y) 
	{
		Vector3 rgb(TargetImg(x, y, 0), TargetImg(x, y, 1), TargetImg(x, y, 2));
		Vector3 lab = RGB_to_LAB(rgb);
		tmpImg2(x, y, 0) = lab.v[0]; tmpImg2(x, y, 1) = lab.v[1]; tmpImg2(x, y, 2) = lab.v[2];
		meanl2 += lab.v[0]; meana2 += lab.v[1]; meanb2 += lab.v[2];
	}
	meanl2 /= w2 * h2; meana2 /= w2 * h2; meanb2 /= w2 * h2;
	cimg_forXY(tmpImg1, x, y) 
	{
		sigmal2 += sqr(tmpImg1(x, y, 0) - meanl2);
		sigmaa2 += sqr(tmpImg1(x, y, 1) - meana2);
		sigmab2 += sqr(tmpImg1(x, y, 2) - meanb2);
	}
	sigmal2 = sqrt(sigmal2 / (double)(w2 * h2));
	sigmaa2 = sqrt(sigmaa2 / (double)(w2 * h2));
	sigmab2 = sqrt(sigmab2 / (double)(w2 * h2));
	cimg_forXY(tmpImg1, x, y) 
	{
		tmpImg1(x, y, 0) = min(max((tmpImg1(x, y, 0) - meanl1) * (sigmal2 / sigmal1) + meanl2, 0.0), 100.0);
		tmpImg1(x, y, 1) = min(max((tmpImg1(x, y, 1) - meana1) * (sigmaa2 / sigmaa1) + meana2, -120.0), 120.0);
		tmpImg1(x, y, 2) = min(max((tmpImg1(x, y, 2) - meanb1) * (sigmab2 / sigmab1) + meanb2, -120.0), 120.0);
	}
	cimg_forXY(resImg, x, y) 
	{
		Vector3 lab(tmpImg1(x, y, 0), tmpImg1(x, y, 1), tmpImg1(x, y, 2));
		Vector3 rgb = LAB_to_RGB(lab);
		resImg(x, y, 0) = rgb.v[0]; resImg(x, y, 1) = rgb.v[1]; resImg(x, y, 2) = rgb.v[2];
	}
	resImg.save("color_mix.bmp");
}
