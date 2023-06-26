#include "ImageProcessing.h"
#include "ColorSpaces.h"
#include "Augmentations.h"
#include <cmath>
#include <QDebug>
#include <iostream>

void imageProcessingFun(
	const QString& prog_name,
	QImage& out_img,
	const QImage& in_img,
	const QVector<double>& params
) {
    int x_size = in_img.size().width();
    int y_size = in_img.size().height();

    out_img = in_img.copy();


    if(prog_name == "Augmentation 1"){//dct lowpass unsharp masking
		uchar threshold = params[0];
		float compress = params[1];
		


        myAugmentation1(out_img.bits(), x_size, y_size);
	}
    else if(prog_name == "Augmentation 2"){//saturation

        uchar threshold = params[0];
        float compress = params[1];



        myAugmentation2(out_img.bits(), x_size, y_size);
    }else if(prog_name == "Augmentation 3"){ //aspect+cut

        uchar threshold = params[0];
        float compress = params[1];

        srand(time(0));

        float randomness_factorY=(rand()%10)/10;

        int newXSize = ceil(x_size * (1));
        int newYSize = ceil(y_size * (2+randomness_factorY));

        if (newXSize % 8 != 0)
               newXSize = newXSize + (8 - newXSize % 8);
        if (newYSize % 8 != 0)
               newYSize = newYSize + (8 - newYSize % 8);

                /* Calculate output image resolution and construct output image object */


        QImage ImgBuff= QImage(newXSize,newYSize,in_img.format());


                /* Perform Sample and hold interpolation  */
        myAugmentation3(in_img.bits(), x_size,y_size,ImgBuff.bits(), newXSize, newYSize);



        //cutting the image

        out_img = ImgBuff.copy(0,newYSize/100*20,x_size,y_size);

    }
    else if(prog_name == "Augmentation 4"){ //oclusions

            uchar threshold = params[0];
            float compress = params[1];



            myAugmentation4(out_img.bits(), x_size, y_size);
    }
    else if(prog_name == "Augmentation 5"){
        uchar threshold = params[0];
        float compress = params[1];



        myAugmentation5(out_img.bits(), x_size, y_size);


    }
    std::cout<<out_img.size().height();

}
