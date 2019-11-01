#include <iostream>
#include<cstdlib>
#include "Image2D.h"
// git comment 
using namespace std;

int main(){
    Image2D A;
    Image2D B;

    //A+=B;// C=a+b;
    FILE *fp = fopen("new_image.bmp","wb+");
    A.EnregistrerImage(fp);
    fclose(fp);


    Image2D lena5("lena24.bmp",1, 1);
    FILE *fp5 = fopen("lena_Black_white.bmp","wb+");
    lena5.EnregistrerImage(fp5);


    Image2D lena("lena24.bmp",1, 1);
    FILE *fp2 = fopen("lena_new.bmp","wb+");
    lena.SeuilImage(fp2);
    lena.applyBinaryMask("mask.bmp","lena_mask.bmp");
    fclose(fp2);


    Image2D lena1("lena24.bmp",1, 1);
    lena1.translation(100,200,"translation_lena24.bmp");

    Image2D lena2("lena24.bmp",1, 1);
    lena2.rotation(45,"rotation_lena.bmp");



    Image2D lena3("lena24.bmp",1, 1);
    lena3.bruit("lena_salt_and_pepper.bmp");


   Image2D lena4("lena24.bmp",1, 1);
   lena4.lowpass_filter("lena_median_filter.bmp",3);

    Image2D lena6("lena24.bmp",1, 1);
    lena6.highpass_filter("lena_highpass_filter.bmp");

    system("lena_median_filter.bmp");
    return 0;
}
