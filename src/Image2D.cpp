#include "Image2D.h"
#include <cstdlib>
#include<math.h>
#include <cmath>
using namespace std;

//header c_est l_attribut entete de notre image.

Image2D::Image2D(){
    nbx = 128;
    nby = 128;
    number_grey_level = 255;
    sizep = nbx*nby;
    dx=1;
    dy=1;
    alloue_partie_dynamique(128,128);

    for(int i=0;i<nbx;i++){
        for(int j=0;j<nby;j++){
            ptr[i][j] = 128;
        }
    }

}

void Image2D::CreateEntete(){
  int fileSize = (sizep*3)+54;
    unsigned char fileSizeBytePerByte[4];
    fileSizeBytePerByte[0] = fileSize >>0;
    fileSizeBytePerByte[1] = fileSize >>8;
    fileSizeBytePerByte[2] = fileSize >>16;
    fileSizeBytePerByte[3] = fileSize >>24;

    //Byte per byte separation of width
    unsigned char nbXBytePerByte[4];
    nbXBytePerByte[0] = nbx >>0;
    nbXBytePerByte[1] = nbx >>8;
    nbXBytePerByte[2] = nbx >>16;
    nbXBytePerByte[3] = nbx>>24;

    //Byte per byte separation of heigth

    unsigned char nbYBytePerByte[4];
    nbYBytePerByte[0] = nby >>0;
    nbYBytePerByte[1] = nby >>8;
    nbYBytePerByte[2] = nby >>16;
    nbYBytePerByte[3] = nby >>24;



    unsigned char enteteLenaToModify[54]={0x42 ,0x4d ,//BM
        fileSizeBytePerByte[0] ,fileSizeBytePerByte[1] ,fileSizeBytePerByte[2] ,fileSizeBytePerByte[3] ,//File size 54+(nbx*nby)*3
       // 0x36 ,0xc0 ,0x0 ,0x0 ,//File size 54+(nbx*nby)*3
        0x0 ,0x0 ,0x0 ,0x0 ,//unused
        0x36 ,0x0 ,0x0 ,0x0 ,//Offset from beginning to file 54 //start of infoheader
        0x28 ,0x0 ,0x0 ,0x0 ,//size of info header=40
        nbXBytePerByte[0] ,nbXBytePerByte[1] ,nbXBytePerByte[2] ,nbXBytePerByte[3] ,//width
        nbYBytePerByte[0] ,nbYBytePerByte[1] ,nbYBytePerByte[2] ,nbYBytePerByte[3] ,//height
        0x1 ,0x0 ,//planes
        0x18 ,0x0 ,//bits per pixel
        0x0 ,0x0 ,0x0 ,0x0 ,//No compression
        0x0 ,0x0 ,0xc ,0x0 ,//Image size, no compression
        //dxValueBytePerByte[0] ,dxValueBytePerByte[1] ,dxValueBytePerByte[2] ,dxValueBytePerByte[3],//horizontal resolution: Pixels/meter
       // dyValueBytePerByte[0] ,dyValueBytePerByte[1] ,dyValueBytePerByte[2] ,dyValueBytePerByte[3],//vertical resolution: Pixels/meter
        0x74 ,0x12 ,0x0 ,0x0 ,//horizontal resolution: Pixels/meter
        0x74 ,0x12 ,0x0 ,0x0 ,//vertical resolution: Pixels/meter
        0x0 ,0x0 ,0x0 ,0x0 ,//Colors Used
        0x0 ,0x0 ,0x0 ,0x0};  //Important Colors

    for(int k=0;k<54;k++){
        enteteX[k]= enteteLenaToModify[k];
    }
}

void Image2D::EnregistrerImage(FILE *fp){
    CreateEntete();
    fwrite(&enteteX,sizeof(unsigned char),54,fp);
    for(int i = 0;i<nbx;i++){
            for(int j = 0;j<nby;j++){
                //if(ptr[i][j]<seuil){ptr[i][j]=0;}
                //else{ptr[i][j]=255;}
                fwrite(&(*this).ptr[i][j],sizeof(unsigned char),1,fp);
                fwrite(&(*this).ptr[i][j],sizeof(unsigned char),1,fp);
                fwrite(&(*this).ptr[i][j],sizeof(unsigned char),1,fp);
            }
    }
}

void Image2D::SeuilImage(FILE *fp){
    int seuil = 130;
    for(int i = 0;i<nbx;i++){
            for(int j = 0;j<nby;j++){
                if(ptr[i][j]<seuil){ptr[i][j]=0;}
                else{ptr[i][j]=255;}
            }
    }
    EnregistrerImage(fp);
    fclose(fp);
    std:cout<<"Seuil image"<<endl;
}

void Image2D::applyBinaryMask(char* mask,char* picture){

        //Ouvrir le mask
        FILE* fp = fopen(mask, "rb");
        unsigned char info1[54];
        fread(info1, sizeof(unsigned char), 54, fp); // entete du mask
        int w = *(int*)&info1[18];
        int h = *(int*)&info1[22];
        int sizee = 3 * w * h;
        unsigned char* img1 = new unsigned char[3*sizee];
        fread(img1,sizeof(unsigned char),3*sizee,fp);
        fclose(fp);
        //ouvrir l'image à masquer:
        FILE* fp1 = NULL;
        fp1 = fopen(picture,"wb");
        int k = 0;
        for(int i = 0;i<w;i++){
            for(int j = 0;j<h;j++){
              if(ptr[i][j]>img1[k]){
                ptr[i][j]=0;
              }
              if(ptr[i][j]>img1[k+1]){
                ptr[i][j]=0;
              }
              if(ptr[i][j]>img1[k+2]){
                ptr[i][j]=0;
              }
              k+=3;
            }
        }
        EnregistrerImage(fp1);
        fclose(fp1);
        delete [] img1;
        img1=NULL;
        std:cout<<"Masque binaire"<<endl;
        }


void Image2D::translation(int tx, int ty, const char* filename){
    printf("Translation\n");
    int a = 0;
    int ** ctr;
    ctr = ptr;
        FILE* fm=NULL;
        fm =fopen(filename,"wb+");
        for(int i=0;i<(*this).nbx-tx;i++){
            for(int j=0;j<(*this).nby-(ty);j++){
                    ctr[i][j]=ptr[i+tx][j+ty];
                            }
        }
        for(int i=0;i<tx;i++){
            for(int j=0;j<(ty);j++){
                ctr[i][j] = 200;
            }
        }
        for(int i=0;i<tx;i++){
            for(int j=ty;j<(nby);j++){
                ctr[i][j] = 200;
            }
        }
        for(int i=tx;i<nbx;i++){
            for(int j=0;j<(ty);j++){
                ctr[i][j] = 200;
            }
        }
        for(int i=0;i<nbx;i++){
            for(int j=0;j<nby;j++){
                ptr[i][j]=ctr[i][j];
            }
        }
        EnregistrerImage(fm);
        fclose(fm);

}
  void Image2D::rotation(double theta, const char* filename)
  {
theta=theta*M_PI/180;
FILE* fp;
fp =fopen(filename,"wb");

  double centreX=((double)nbx)/2;
  double centreY=((double)nby)/2;
  int x1=0;
  int y1=0;
  Image2D C(nbx,nby,255,1,1,0);

    for(double i = 0;i<nbx;i++){
            for(double j = 0;j<nby;j++){

                    x1 = (int)((cos(theta)*(i-centreX)-sin(theta)*(j-centreY)+centreX));
                    y1 = (int)((sin(theta)*(i-centreX)+cos(theta)*(j-centreY)+centreY));

                    if(x1<nbx&&x1>=0&&y1<nby&&y1>=0){
                    C.ptr[x1][y1]=(*this).ptr[(int)i][(int)j];
                    }
            }
    }

    CreateEntete();
    fwrite(&enteteX,sizeof(unsigned char),54,fp);
    for(int i = 0;i<nbx;i++){
            for(int j = 0;j<nby;j++){
                fwrite(&C.ptr[i][j],sizeof(unsigned char),1,fp);
                fwrite(&C.ptr[i][j],sizeof(unsigned char),1,fp);
                fwrite(&C.ptr[i][j],sizeof(unsigned char),1,fp);
            }
    }
    fclose(fp);
std::cout<<"Rotation"<<endl;
  }

/*Image2D& Image2D::operator+=(const Image2D & p2)
 {

   (*this).nbx+=p2.nbx;
   (*this).nby+=p2.nby;
   (*this). number_grey_level += p2.number_grey_level;
   (*this). sizep += p2.sizep;
   (*this). dx+=p2.dx;
   (*this). dy+=p2.dy;
          for(int i=0;i<nbx;i++){
        for(int j=0;j<nby;j++){
            ptr[i][j] += p2.ptr[i][j];
        }
    }

 }*/

void Image2D::bruit(const char* filename){
    FILE* fp = fopen(filename,"wb");
    int L[2] = {0,255};

    for(int i=0;i<nbx;i++){
        for(int j=0;j<nby;j++){
                int a = rand()%100;
                if(a<=2){
                        ptr[i][j] = L[rand()%2];
                 }
        }
    }
    EnregistrerImage(fp);
    fclose(fp);
     cout<<"salt and pepper"<<endl;
}

void Image2D::lowpass_filter(const char* filename,  int m ){
    FILE* fp = fopen(filename,"wb+");
    int ** ctr = new int*[nbx];
       for(int i=0; i<nbx; i++){
            ctr[i] = new int [nby];
            }

    for(int i=0; i<nbx; i++){
            for(int j=0; j<nby; j++){
            int l = 0;
            int k = 0;
            int sum = 0;
                while(k<m){
                        while(l<m){
                                if((i+k<nbx)&&(j+l<nby)){
                                     ctr[i][j]+= ptr[i+k][j+l];}
                                    l++;}
                k++;
            }
    }}
    for(int i = 0;i<nbx;i++){
        for(int j=0;j<nby;j++) {
             ptr[i][j] = int(ctr[i][j]/(m*m));
    }}
        EnregistrerImage(fp);
    fclose(fp);
    cout<<"lowpass filter"<<endl;
}

void Image2D::highpass_filter(const char* filename, int m){
    FILE* fp = fopen(filename,"wb+");
    /*for(int i=1;i<nbx-1;i++){
            for(int j=1;j<nby-1;j++){
                  double dp =   (-ptr[i-1][j-1]-ptr[i-1][j]
                                    -ptr[i-1][j+1]-ptr[i][j-1]+
                                    8*ptr[i][j]-ptr[i][j+1]
                                    -ptr[i+1][j-1]-ptr[i+1][j]
                                    -ptr[i+1][j+1])/9;
                 ptr[i][j] = int (dp);
                 }
        }*/
        int ** ctr;
    ctr = ptr;
       for(int i=0; i<nbx; i++){
            for(int j=0; j<nby; j++){
                ctr[i][j] = ptr[i][j];
            }}
int l=-m+1;
int k=-m+1;
   for(int i=0; i<nbx; i++){
            for(int j=0; j<nby; j++){
                    for(int k=-m+1;k<=(m-1);k++){
                          if((i+k<nbx)&&(i+k)>=0){
                            for(int l=-m+1;l<=(m-1);l++){
                                  if((j+l<nby)&&(j+l)>=0){
                                     (*this).ptr[i][j] +=- int(ctr[i+k][j+l]/(m*m));
                                     }
                    }
                    }
        }
        (*this).ptr[i][j]= int((*this).ptr[i][j])+ctr[i][j];
    }
   }
    EnregistrerImage(fp);
    fclose(fp);
    std:cout<<"High pass filter"<<endl;
}

/*void Image2D::EnregistrerImage_RGB(FILE* fp){
    CreateEntete();

     int ** ptr_G = new int*[nbx];
     int ** ptr_B = new int*[nbx];
     int ** ptr_R = new int*[nbx];

       for(int i=0; i<nbx; i++){
            ptr_G[i] = new int [nby];
            ptr_B[i] = new int [nby];
            ptr_R[i] = new int [nby];
            if ( ptr_B==NULL || ptr_B[i]==NULL || ptr_G==NULL || ptr_G[i]==NULL || ptr_R==NULL || ptr_R[i]==NULL )
        {
            std::cout<<"Probleme allocation memoire...exiting\n";
            exit ( 1 );_
        }
            }
            for(int i=0;i<nbx;i++){
                for(int j=0; j<nby;j++){

                    ptr_B[i][j] = (int)(double)(*this).imageBLU[i*((*this).nby)+j];
                    ptr_G[i][j] = (int)(double)(*this).imageGRE[i*((*this).nby)+j];
                    ptr_R[i][j] = (int)(double)(*this).imageRED[i*((*this).nby)+j];
                }
            };

    fwrite(&enteteX,sizeof(unsigned char),54,fp);
    for(int i = 0;i<nbx;i++){
            for(int j = 0;j<nby;j++){
                fwrite(&ptr[i][j],sizeof(unsigned char),1,fp);
                fwrite(&ptr[i][j],sizeof(unsigned char),1,fp);
                fwrite(&ptr[i][j],sizeof(unsigned char),1,fp);
            }
    }
}*/
Image2D::Image2D(int nx,int ny,int ngl,double milx,double mily,int p0){

   (*this). nbx = nx;
   (*this). nby = ny;
   (*this). number_grey_level= ngl;
   (*this). dx  = milx;
   (*this). dy  = mily;
       alloue_partie_dynamique(nbx,nby);
    for(int i=0;i<nbx;i++){
        for(int j=0;j<nby;j++){
            ptr[i][j] = p0;
        }
    }
}

Image2D::Image2D(char* path,double milx, double mily){
    FILE* f = fopen(path, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f);
    // read the 54-byte header
    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    int sizep = 3 * width * height;
    (*this).nbx=width;
    (*this).nby=height;
    (*this).sizep=width*height;
    (*this).dx=milx;
    (*this).dy=mily;
    unsigned char* image = new unsigned  char[sizep]; // allocate 3 bytes per pixel
    unsigned char* imageBLU=new unsigned char[width*height];//definition of the RED
    unsigned char* imageGRE=new unsigned char[width*height];//definition of the GREEN
    unsigned char* imageRED=new unsigned char[width*height];//definition of the BLUE

    fread(image,sizeof(unsigned char),sizep,f);//We fill the FULL image
    fclose(f);

    for(int id=0;id<(width*height);id++)
    {
        imageBLU[id]=image[3*id];
        imageGRE[id]=image[3*id+1];
        imageRED[id]=image[3*id+2];

    }
    (*this).alloue_partie_dynamique(width,height);
    for (int i=0; i<(*this).nbx; i++) {
        for (int j=0; j<(*this).nby; j++) {
            double mean=0;
            mean=((double)imageRED[i*((*this).nby)+j]+(double)imageBLU[i*((*this).nby)+j]+(double)imageGRE[i*((*this).nby)+j])/3.0;
                (*this).ptr[i][j]=(int)mean;
        }
    }

    delete [] image;
    image=NULL;
    delete [] imageBLU;
    imageBLU=NULL;
    delete [] imageRED;
    imageRED=NULL;
    delete [] imageGRE;
    imageGRE=NULL;


}

int Image2D::getNbx(){
    return nbx;
}
void Image2D::setNbx(int a){
    (*this).nbx=a;

}
int Image2D::getNby(){
    return nby;
}
void Image2D::setNby(int a){
    (*this).nby=a;
}
int Image2D::getNgl(){
    return number_grey_level;
}
void Image2D::setNgl(int a){
    (*this).number_grey_level=a;
}
long int Image2D::getSize(){
    return sizep;
}
double Image2D::getDx(){
    return dx;
}
void Image2D::setDx(double a){
    (*this).dx=a;
}
double Image2D::getDy(){
    return dy;
}
void Image2D::setDy(double a){
    (*this).dy=a;
}

int Image2D::getPixelValue(int i,int j){
    return (*this).ptr[i][j];
}
void Image2D::setPixelValue(int i,int j, int a){
 (*this).ptr[i][j]=a;
}


// allocation dynamique de mémoire via new..
void Image2D::alloue_partie_dynamique(int dimx, int dimy){
    if ( dimx<=0 || dimy<=0 )
    {
        std::cout<<"Dimension de l'allocation anormales...\n";
        std::cout<<"dimx : " << dimx << ", dimy :" << dimy << endl ;
        exit ( 1 );
    }

    ptr = new int* [dimx]; //!< ptr[i] pointe sur la ligne i.
    for (int i=0; i<dimx ; i++){
        ptr[i] = new int[dimy]; //!< ptr[i][j] sur l'element i,j
        if ( ptr==NULL || ptr[i]==NULL )
        {
            std::cout<<"Probleme allocation memoire...exiting\n";
            exit ( 1 );
        }
    }

}

void Image2D::libere_partie_dynamique(){
    if ( ptr!=NULL)
    {
        for(int i=0; i< ((*this).nbx) ; i++){
            if ( ptr[i]!=NULL ){
                delete [] ptr[i];
            }
        }
        delete [] ptr;
        ptr=NULL;
    }
}

void Image2D::init(int valeur_init){
    if ((*this).ptr!=NULL)
    {
        for (int i=0; i<(*this).nbx; i++) {
            for (int j=0; j<(*this).nby; j++) {
                (*this).ptr[i][j]=valeur_init;
            }
        }
    }else{
        (*this).alloue_partie_dynamique((*this).nbx,(*this).nby);
        for (int i=0; i<(*this).nbx; i++) {
            for (int j=0; j<(*this).nby; j++) {
                (*this).ptr[i][j]=valeur_init;
            }
        }
    }
}
Image2D::~Image2D()
{
    (*this).libere_partie_dynamique();
}
