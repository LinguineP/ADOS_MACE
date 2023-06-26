#include "ImageFilter.h"
#include <cmath>


void convolve2D (uchar image[], int x_size, int y_size, double filterCoeff[], int N) {
	//TO DO
    int delta = (N-1)/2;
    int new_x_size = x_size + 2 * delta;
    int new_y_size = y_size + 2 * delta;

    //1. zauzimamo niz za novu prosirenu sliku
    uchar* extendedImage = new uchar[new_x_size * new_y_size];

    //2. pozovemo extendedBorders
    extendBorders(image, x_size, y_size, extendedImage, delta);


    for(int y = 0; y < y_size; ++y){
        for(int x = 0; x < x_size; ++x){
            double accum = 0;
            //3. za svaki red, po svakom pikselu u redu, radimo filtriranje
            for (int yy = 0; yy < N; yy++){
                for (int xx = 0; xx < N; xx++){
                    accum += extendedImage[(y+yy)*new_x_size + x+xx] * filterCoeff[(N-yy)*N -xx-1];
                }
            }
            if (accum > 255.0){
                image[y * x_size + x] = 255;
            }else if (accum < 0.0){
                image[y * x_size + x] = 0;
            }else{
                image[y * x_size + x] = floor(accum+0.5);
            }
        }
    }

     delete[] extendedImage;
}

void extendBorders(uchar input[], int x_size, int y_size, uchar output[], int delta) {
	int new_x_size = x_size + 2*delta;
	int new_y_size = y_size + 2*delta;

	for (int y = 0; y < y_size; y++){
		for(int x = 0; x < x_size; x++){
			output[(y+delta) * new_x_size +x+delta] = input[y*x_size + x];
		}
	}

	for (int yy = 0; yy < delta; yy++){
		memcpy(
			&output[yy * new_x_size + delta],
			&input[0],
			x_size
		);
		memcpy(
			&output[(y_size+delta+yy) * new_x_size + delta],
			&input[(y_size-1)*x_size],
			x_size
		);
	}

	for (int y = 0; y < new_y_size; y++){
		memset(
			&output[y * new_x_size],
			output[y * new_x_size + delta],
			delta
		);
		memset(
			&output[y * new_x_size + delta+x_size],
			output[y * new_x_size + x_size+delta-1],
			delta
		);
	}


}

void performNFFilter (uchar input[], int x_size, int y_size) {
	//TO DO
    //1.napraviti kernel za NF, matriicu koja je tri puta tri i postavimo je na jendu devetinu
    double filterNFCoef[] = {1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0,};
    //2.pozovemo convolve 2D
    convolve2D(input, x_size, y_size, filterNFCoef, 3);

}

void performVFFilter (uchar input[], int x_size, int y_size) {
	//TO DO
    double filterVFCoef[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};

    convolve2D(input, x_size, y_size, filterVFCoef, 3);
}

void performSuccessiveNFFilter (uchar input[], int x_size, int y_size, int stages) {
	//TO DO
    double filterNFCoef[] = {1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/9.0,};
    int i = 0;

    while(i < stages){
        convolve2D(input, x_size, y_size, filterNFCoef, 3);
        i++;
    }
}
