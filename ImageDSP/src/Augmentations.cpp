#include "Augmentations.h"
#include "NxNDCT.h"
#include <cmath>
#include "ImageFilter.h"
#include "ColorSpaces.h"



using namespace std;


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void performUnsharpMasking(uchar Y_buff[],int x_size,int y_size,
                           uchar threshold){

    uchar* Y_original=new uchar[x_size*y_size];

    //saving the original
    for(int i = 0; i < y_size*x_size; i++){
        Y_original[i]=Y_buff[i];
    }

    //get blurred img with nf filtering
    performNFFilter(Y_buff,x_size,y_size);



    //unsharp masking
    for(int i = 0; i < y_size*x_size; i++){
        Y_buff[i]=Y_original[i]+(Y_original[i]-Y_buff[i])*(threshold/255);
    }


    delete[] Y_original;



}

void performDCTandIDCT(
    uchar* input,
    int x_size,
    int y_size,
    int N,
    uchar treshold
) {
    // Create NxN buffer for one input block
    uchar* inBlock = new uchar[N*N];

    // Generate DCT kernel

    double* DCTKernel = new double[N*N];
    GenerateDCTmatrix(DCTKernel, N);

    // Create buffer for DCT coefficients
    short* dctCoeffs = new short[N*N];

    // Extend image borders
    int x_size2, y_size2;
    uchar* input2;
    extendBorders(input, x_size , y_size, N, &input2, &x_size2, &y_size2);

    for(int y = 0; y < y_size2/N; y++){
        for(int x = 0; x < x_size2/N; x++){

            // Fill input block buffer
            for(int j=0; j<N; j++){
                for(int i=0; i<N; i++){
                    inBlock[j*N+i] = input2[(N*y+j)*(x_size2)+N*x+i];
                }
            }
            //invoke dct
            DCT(inBlock, dctCoeffs, N, DCTKernel);


            //call usharp masking on the dct
            performUnsharpMasking((unsigned char*)dctCoeffs,x_size,y_size,treshold);

            // Invoke IDCT
            IDCT(dctCoeffs, inBlock, N, DCTKernel);

            // Write output values to output image
            for(int j=0; j<N; j++){
                for(int i=0; i<N; i++){
                    input2[(N*y+j)*(x_size2)+N*x+i] = inBlock[j*N+i];
                }
            }
        }
    }

    cropImage(input2, x_size2, y_size2, input, x_size, y_size);

    // Delete used memory buffers coefficients
    delete[] dctCoeffs;
    delete[] inBlock;
    delete[] DCTKernel;
    delete[] input2;
}




//\\DCT lowpass Unsharp masking

void myAugmentation1(
    uchar in_out_y[],
    int x_size,
    int y_size
) {
    //dct and unsharp masking
    int N=100;
    uchar treshold=10;

    uchar* IMG_buffer = new uchar[x_size*y_size];
    IMG_buffer=(uchar*)in_out_y;

    // Create buffers for YUV image.
    uchar* R_buffer = new uchar[x_size*y_size];
    uchar* G_buffer = new uchar[x_size*y_size];
    uchar* B_buffer = new uchar[x_size*y_size];



    int h=y_size/1;
    int w=x_size/1;





    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            R_buffer[y*w + x] = IMG_buffer[(y*x_size + x)*3 + 0];
            G_buffer[y*w + x] = IMG_buffer[(y*x_size + x)*3 + 1];
            B_buffer[y*w + x] = IMG_buffer[(y*x_size + x)*3 + 2];
        }
    }







    performDCTandIDCT(R_buffer,x_size,y_size,N,treshold);
    performDCTandIDCT(G_buffer,x_size,y_size,N,treshold);
    performDCTandIDCT(B_buffer,x_size,y_size,N,treshold);



    for(int y = 0; y < y_size; y++){
        for(int x = 0; x < x_size; x++){
            in_out_y[(y*x_size + x)*3 + 0]=R_buffer[y*w + x];
            in_out_y[(y*x_size + x)*3 + 1]=G_buffer[y*w + x];
            in_out_y[(y*x_size + x)*3 + 2]=B_buffer[y*w + x];
        }
    }

    delete[] R_buffer;
    delete[] G_buffer;
    delete[] B_buffer;

}



//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//changes individual pixel values constants .299, .587 .114   represent effects
//of r g b components on percived brightness

void changeSaturation(uchar *R, uchar *G, uchar *B, double change) {

    double  Pr = 0.299;
    double  Pg = 0.587;
    double  Pb = 0.114;

    double tmpR,tmpG,tmpB;

    double  P=sqrt((*R)*(*R)*Pr+(*G)*(*G)*Pg+(*B)*(*B)*Pb);


          tmpR=P+((*R)-P)*change;

          if (tmpR > 255.0){
              *R = 255;
          }else if (tmpR < 0.0){
              *R = 0;
          }else{
              *R = floor(tmpR+0.5);
          }


          tmpG=P+((*G)-P)*change;
          if (tmpG > 255.0){
             *G = 255;
          }else if (tmpG < 0.0){
              *G = 0;
          }else{
              *G = floor(tmpG+0.5);
          }

          tmpB=P+((*B)-P)*change;


          if (tmpB > 255.0){
              *B = 255;
          }else if (tmpB < 0.0){
              *B = 0;
          }else{
              *B = floor(tmpB+0.5);
          }
}


//\\saturation
// change the saturation of an rgb image, using HSP system

void myAugmentation2(
    uchar in_out_y[],
    int x_size,
    int y_size
) {

    //from 1.0 to 2.0   boosts sturation from 0.0 to 1.0 cuts
    double saturation_factor=2.0;


    uchar* IMG_buffer = new uchar[x_size*y_size];
    IMG_buffer=(uchar*)in_out_y;

    // Create buffers for YUV image.
    uchar* R_buffer = new uchar[x_size*y_size];
    uchar* G_buffer = new uchar[x_size*y_size];
    uchar* B_buffer = new uchar[x_size*y_size];




    int h=y_size/1;
    int w=x_size/1;


    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            R_buffer[y*w + x] = IMG_buffer[(y*x_size + x)*3 + 0];
            G_buffer[y*w + x] = IMG_buffer[(y*x_size + x)*3 + 1];
            B_buffer[y*w + x] = IMG_buffer[(y*x_size + x)*3 + 2];
        }
    }


    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            changeSaturation(
            &R_buffer[y*w + x],
            &G_buffer[y*w + x],
            &B_buffer[y*w + x],
                    saturation_factor);
        }
    }



    for(int y = 0; y < y_size; y++){
        for(int x = 0; x < x_size; x++){
           in_out_y[(y*x_size + x)*3 + 0]=R_buffer[y*w + x];
           in_out_y[(y*x_size + x)*3 + 1]=G_buffer[y*w + x];
           in_out_y[(y*x_size + x)*3 + 2]=B_buffer[y*w + x];
        }
    }





    // Delete used memory buffers.
    delete[] R_buffer;
    delete[] G_buffer;
    delete[] B_buffer;

}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


void get_start_point(int width,int height,int* x,int* y){

    *x=(width/100*20)+(rand()%(width/100*40)+1);
    *y=(height/100*20)+(rand()%(height/100*40)+1);

}



int calculate_oclusion_width(int width,int x){
    int ret;


    ret=((width-x)/100*50)/100*((rand()+40)%100);

    if(ret<40){
        ret=40;
    }

    return ret;
}

int calculate_oclusion_height(int height,int y){

    int ret;

    ret=((height-y)/100*50)/100*((rand()+40)%100);

    if(ret<40){
        ret=40;
    }

    return ret;
}

void draw_oclusion(uchar* R, uchar* G, uchar* B, int startX, int startY, int img_width, int img_height){


    //geting a random oclusion color
    uchar color_R=rand()%256;
    uchar color_G=rand()%256;
    uchar color_B=rand()%256;

    int oclusionHeight=calculate_oclusion_height(img_height,startY);
    int oclusionWidth=calculate_oclusion_width(img_width,startX);

    for(int y = 0; y < img_height; y++){
        for(int x = 0; x < img_width; x++){
            if(x>startX && x<startX+oclusionWidth){
                if(y>startY && y<startY+oclusionHeight){
                    R[y*img_width + x] = color_R;
                    G[y*img_width + x] = color_G;
                    B[y*img_width + x] = color_B;
                }
            }
       }
    }

}




//\\occlusions

void myAugmentation4(
    uchar in_out_y[],
    int x_size,
    int y_size
) {

    srand(time(0));

    uchar* IMG_buffer = new uchar[x_size*y_size*3];
    IMG_buffer=(uchar*)in_out_y;

    // Create buffers for YUV image.
    uchar* R_buffer = new uchar[x_size*y_size];
    uchar* G_buffer = new uchar[x_size*y_size];
    uchar* B_buffer = new uchar[x_size*y_size];


    int h=y_size/1;
    int w=x_size/1;



    int startX,startY;

    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            R_buffer[y*w + x] = IMG_buffer[(y*x_size + x)*3 + 0];
            G_buffer[y*w + x] = IMG_buffer[(y*x_size + x)*3 + 1];
            B_buffer[y*w + x] = IMG_buffer[(y*x_size + x)*3 + 2];
        }
    }

   get_start_point(w,h,&startX,&startY);

   draw_oclusion(R_buffer,G_buffer,B_buffer,startX,startY,w,h);


   get_start_point(w,h,&startX,&startY);

   draw_oclusion(R_buffer,G_buffer,B_buffer,startX,startY,w,h);



    for(int y = 0; y < y_size; y++){
        for(int x = 0; x < x_size; x++){
           in_out_y[(y*x_size + x)*3 + 0]=R_buffer[y*w + x];
           in_out_y[(y*x_size + x)*3 + 1]=G_buffer[y*w + x];
           in_out_y[(y*x_size + x)*3 + 2]=B_buffer[y*w + x];
        }
    }





    // Delete used memory buffers.
    delete[] R_buffer;
    delete[] G_buffer;
    delete[] B_buffer;

}




//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\




//\\augmentation3 aspect+cut



void myAugmentation3(
    const uchar input[],
    int x_size,
    int y_size,
    uchar output[],
    int new_x_size,
    int new_y_size
) {

    //just interpolate

    double Fx = ((double)new_x_size) / x_size;
    double Fy = ((double)new_y_size) / y_size;

    for(int y = 0; y < new_y_size; y++){
        for(int x = 0; x < new_x_size; x++){
            int xx = floor((x - Fx / 2) / Fx + 1);
            int yy = floor((y - Fy / 2) / Fy + 1);

            if(xx >= x_size){
                xx = x_size - 1;
            }
            if(yy >= y_size){
                yy = y_size - 1;
            }

            for(int c = 0; c < 3; c++){
                output[(y * new_x_size  + x) * 3 + c] =
                    input[(yy * x_size  + xx) * 3 + c];
            }
        }
    }







}


//zigzag

static void doZigZag(short block[], int N, int B)
{
    /* TO DO */
    int currNum = 0;
    int currDiagonalWidth = 1;
    int i;
    int row;
    int col;

    while (currDiagonalWidth<N)
    {
        for (i = 0; i<currDiagonalWidth; i++)
        {
            if (currDiagonalWidth % 2 == 1)
            {
                row = currDiagonalWidth - i - 1;
                col = i;
            }
            else
            {
                row = i;
                col = currDiagonalWidth - i - 1;
            }

            if(currNum > B)
                block[row*N + col] = 0;
            currNum++;
        }
        currDiagonalWidth++;
    }
    while (currDiagonalWidth> 0)
    {
        for (i = currDiagonalWidth; i> 0; i--)
        {
            if (currDiagonalWidth % 2 == 1){
                row = N - currDiagonalWidth + i - 1;
                col = N - i;
            }
            else
            {
                row = N - i;
                col = N - currDiagonalWidth + i - 1;
            }

            if(currNum > B)
                block[row*N + col] = 0;
            currNum++;
        }

        currDiagonalWidth--;
    }

}


/* perform masking of DCT coeffs */
void performMaskDCTCoeffs(uchar input[], int xSize, int ySize, int N, int B)
{
    /* TO DO */
    /* Create NxN buffer for one input block */
    uchar* inBlock = new uchar[N*N];

    /* Generate DCT kernel */
    double* DCTKernel = new double[N*N];
    GenerateDCTmatrix(DCTKernel, N);

    /* Create buffer for DCT coefficients */
    short* dctCoeffs = new short[N*N];

    /* Extend image borders */
    int xSize2, ySize2;
    uchar* input2;
    extendBorders(input, xSize , ySize, N, &input2, &xSize2, &ySize2);

    for (int y = 0; y < ySize2/N; y++)
    {
        for (int x = 0; x < xSize2/N; x++)
        {

            /* Fill input block buffer */
            for (int j=0; j<N; j++)
            {
                for (int i=0; i<N; i++)
                {
                    inBlock[j*N+i] = input2[(N*y+j)*(xSize2)+N*x+i];
                }
            }

            /* Invoke DCT */
            DCT(inBlock, dctCoeffs, N, DCTKernel);

            doZigZag(dctCoeffs, N, B);

            /* Invoke IDCT */
            IDCT(dctCoeffs, inBlock, N, DCTKernel);

            /* Write output values to output image */
            for (int j=0; j<N; j++)
            {
                for (int i=0; i<N; i++)
                {
                    input2[(N*y+j)*(xSize2)+N*x+i] = inBlock[j*N+i];
                }
            }
        }
    }

    cropImage(input2, xSize2, ySize2, input, xSize, ySize);

    /* Delete used memory buffers coefficients */
    delete[] dctCoeffs;
    delete[] inBlock;
    delete[] DCTKernel;
    delete[] input2;

}


void myAugmentation5(
    uchar in_out_y[],
    int x_size,
    int y_size
) {


    int N = 70;
    int B = 70;
    float lambda = 0.2;

    uchar* IMG_buffer = new uchar[x_size*y_size];
    IMG_buffer=(uchar*)in_out_y;

    // Create buffers for YUV image.
    uchar* R_buffer = new uchar[x_size*y_size];
    uchar* G_buffer = new uchar[x_size*y_size];
    uchar* B_buffer = new uchar[x_size*y_size];



    int h=y_size/1;
    int w=x_size/1;





    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            R_buffer[y*w + x] = IMG_buffer[(y*x_size + x)*3 + 0];
            G_buffer[y*w + x] = IMG_buffer[(y*x_size + x)*3 + 1];
            B_buffer[y*w + x] = IMG_buffer[(y*x_size + x)*3 + 2];
        }
    }



    performMaskDCTCoeffs(R_buffer, x_size, y_size, N, B);
    performMaskDCTCoeffs(G_buffer, x_size, y_size, N, B);
    performMaskDCTCoeffs(B_buffer, x_size, y_size, N, B);



    for(int y = 0; y < y_size; y++){
        for(int x = 0; x < x_size; x++){
            in_out_y[(y*x_size + x)*3 + 0]=in_out_y[(y*x_size + x)*3 + 0]+(in_out_y[(y*x_size + x)*3 + 0]-R_buffer[y*w + x])*lambda;
            in_out_y[(y*x_size + x)*3 + 1]=in_out_y[(y*x_size + x)*3 + 1]+(in_out_y[(y*x_size + x)*3 + 1]-G_buffer[y*w + x])*lambda;
            in_out_y[(y*x_size + x)*3 + 2]=in_out_y[(y*x_size + x)*3 + 2]+(in_out_y[(y*x_size + x)*3 + 2]-B_buffer[y*w + x])*lambda;
        }
    }

    delete[] R_buffer;
    delete[] G_buffer;
    delete[] B_buffer;

}
