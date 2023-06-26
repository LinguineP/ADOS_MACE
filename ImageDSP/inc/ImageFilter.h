#ifndef IMAGE_FILTER_H_
#define IMAGE_FILTER_H_
#include <QDebug>

void convolve2D (uchar Y_buff[], int xSize, int ySize, double filterCoeff[], int N);

void extendBorders(uchar input[], int xSize, int ySize, uchar output[], int delta);
	
void performNFFilter (uchar input[], int xSize, int ySize);

void performVFFilter (uchar input[], int xSize, int ySize);

void performSuccessiveNFFilter (uchar input[], int xSize, int ySize, int stages);

#endif // IMAGE_FILTER_H_
