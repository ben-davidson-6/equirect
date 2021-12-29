#include <Eigen/Dense>
#include "image.h"
#include <math.h>
#include "sphere.h"
#include <iostream>

EquirectangularImage interpolate(EquirectangularImage original, Eigen::ArrayXXd coords){
    int offset, top, bot, left, right;
    double y, x, fxTop, fxBot, interpolated;
    unsigned char q00, q01, q10, q11;
    const CImg<unsigned char> image = original.getImage();
    const unsigned char* imageBuffer = image.data();
    unsigned char interpolatedImage[original.width*original.height*3];
    int channelOffset, bufferOffset, height, width;
    height = original.height;
    width = original.width;

    for (int channel = 0; channel < 3; channel++) {
        channelOffset = channel*height*width;
        for (int i = 0; i < height; i++) {
            bufferOffset = i*width + channelOffset;
            offset = i*width;
            
            for (int j = 0; j < width; j++) {
                y = coords(offset + j, 0);
                x = coords(offset + j, 1);
    
                top = static_cast <int> (std::floor(y));
                bot = std::min(height - 1, static_cast <int> (std::ceil(y)));
                left = static_cast <int> (std::floor(x));
                right = (static_cast <int> (std::ceil(x)))%width;
    
                q00 = imageBuffer[channelOffset + top*width + left];
                q01 = imageBuffer[channelOffset + top*width + right];
                q10 = imageBuffer[channelOffset + bot*width + left];
                q11 = imageBuffer[channelOffset + bot*width + right];
    
                if (left != right) {
                    fxTop = (right - x)*q00 + (x - left)*q01;
                    fxBot = (right - x)*q10 + (x - left)*q11;
                } else {
                    fxTop = q00;
                    fxBot = q10;
                }
                interpolated = (y - top)*fxBot + (bot - y)*fxTop;
                interpolatedImage[channelOffset + i*width + j] = (unsigned char)interpolated;
            }
        }
    }
    CImg<unsigned char> cInterpedImage(interpolatedImage, width, height, 1, 3, true);
    return EquirectangularImage(cInterpedImage);
}

EquirectangularImage rotate(EquirectangularImage image, Eigen::Matrix3d rotation){
    EquirectangularSphere sphere(image.height, image.width);
    SphericalCoordinates sphericalCoords = sphere.rotate(rotation);
    return interpolate(image, sphericalCoords.toEquirectangularCoords());
}