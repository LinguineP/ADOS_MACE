
#pragma once

#include <QDebug>



void myAugmentation1(
	uchar in_out_rgb[],
	int x_size,
    int y_size
);


void myAugmentation2(
    uchar in_out_y[],
    int x_size,
    int y_size
);

void myAugmentation3(
    const uchar input[],
    int x_size,
    int y_size,
    uchar output[],
    int new_x_size,
    int new_y_size
);

void myAugmentation4(
    uchar in_out_y[],
    int x_size,
    int y_size
);

void myAugmentation5(
    uchar in_out_y[],
    int x_size,
    int y_size
) ;
