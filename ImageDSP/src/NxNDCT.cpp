#define _USE_MATH_DEFINES
#include <math.h>
#include "NxNDCT.h"

#define PI 3.14159265359

void GenerateDCTmatrix(double* DCTKernel, int order) {
	int i, j;
	int N;
	N = order;
	double alpha;
	double denominator;
	for(j = 0; j <= N - 1; j++){
		DCTKernel[0, j] = sqrt(1 / (double)N);
	}
	alpha = sqrt(2 / (double)N);
	denominator = (double)2 * N;
	for(j = 0; j <= N - 1; j++){
		for(i = 1; i <= N - 1; i++){
			DCTKernel[i*N + j] = 
				alpha * 
				cos(
					((2 * j + 1) * i * PI) 
						/ denominator
				);
		}
	}
}

void DCT(
	const uchar input[],
    short output[],
	int N,
	double* DCTKernel
) {
	double* temp = new double[N*N];
	double* DCTCoefficients = new double[N*N];

	double sum;
	for(int i = 0; i <= N - 1; i++){
		for(int j = 0; j <= N - 1; j++){
			sum = 0;
			for(int k = 0; k <= N - 1; k++){
				sum = sum 
					+ DCTKernel[i*N+k]
					* (input[k*N+j]-128.0);
			}
			temp[i*N + j] = sum;
		}
	}

	for(int i = 0; i <= N - 1; i++){
		for(int j = 0; j <= N - 1; j++){
			sum = 0;
			for(int k = 0; k <= N - 1; k++){
                sum = sum + temp[i*N+k] * DCTKernel[j*N+k];
			}
			DCTCoefficients[i*N+j] = sum;
		}
	}
	
	for(int i = 0; i < N*N; i++){
		output[i] = floor(DCTCoefficients[i]+0.5);
	}
		
	delete[] temp;
	delete[] DCTCoefficients;
}

void IDCT(
    const short input[],
	uchar output[],
	int N,
	double* DCTKernel
) {
	// TODO
    double* t = new double[N*N];
    double* DCTCoefficients = new double[N*N];

    double sum;
    for(int i = 0; i <= N - 1; i++){
        for(int j = 0; j <= N - 1; j++){
            sum = 0;
            for(int k = 0; k <= N - 1; k++){
                sum = sum
                    + DCTKernel[k*N+i]
                    * (input[k*N+j]);
            }
            t[i*N + j] = sum;
        }
    }

    for(int i = 0; i <= N - 1; i++){
        for(int j = 0; j <= N - 1; j++){
            sum = 0;
            for(int k = 0; k <= N - 1; k++){
                sum = sum + t[i*N+k] * DCTKernel[k*N+j];
            }
            DCTCoefficients[i*N+j] = sum;
        }
    }

    for(int i = 0; i < N*N; i++){
        if(floor(DCTCoefficients[i]+0.5)>255)
            output[i] = 255;
        else if(floor(DCTCoefficients[i]+0.5)<0)
            output[i] = 0;
    }


    delete[] t;
    delete[] DCTCoefficients;
}



void extendBorders(
	uchar* input,
	int x_size,
	int y_size,
	int N,
	uchar** p_output,
	int* new_x_size,
	int* new_y_size
) {
	int delta_x = N-x_size%N;
	int delta_y = N-y_size%N;

	*new_x_size = x_size+delta_x;
	*new_y_size = y_size+delta_y;

	uchar* output = new uchar[(x_size+delta_x)*(y_size+delta_y)];

	for(int i=0; i<y_size; i++){
		memcpy(
			&output[i*(x_size+delta_x)],
			&input[i*(x_size)],
			x_size
		);
		if(delta_x != 0){
			memset(
				&output[i*(x_size+delta_x)+x_size],
				output[i*(x_size+delta_x)+x_size - 1],
				delta_x
			);
		}
	}

	for(int i=0; i<delta_y; i++){
		memcpy(
			&output[(i + y_size)*(x_size+delta_x)],
			&output[(y_size)*(x_size+delta_x)],
			x_size+delta_x
		);
	}

	*p_output = output;
}


void cropImage(
	uchar* input,
	int x_size,
	int y_size,
	uchar* output,
	int new_x_size,
	int new_y_size
) {
	for(int i=0; i<new_y_size; i++){
		memcpy(
			&output[i*(new_x_size)],
			&input[i*(x_size)],
			new_x_size
		);
	}
}





