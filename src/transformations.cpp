#include <Eigen/Dense>
#include "image.h"
#include <math.h>
#include "sphere.h"
#include <iostream>

typedef Eigen::Array<int, -1, -1, Eigen::RowMajor> ArrayIRow ;
typedef Eigen::Array<double, -1, -1, Eigen::RowMajor> ArrayDRow;
typedef std::pair<ArrayDRow, ArrayIRow> PixelCoords;


PixelCoords calculatePixelCoords(const Eigen::ArrayXXd &coords, int height, int width) {
    Eigen::VectorXd y = coords.col(0);
    double widthd = 1.0*width;
    Eigen::VectorXd x = coords.col(1).unaryExpr([widthd](double x) { return std::fmod(x, widthd); });
    
    Eigen::VectorXd top = y.array().floor(); 
    Eigen::VectorXd bot = y.array().ceil().cwiseMin(height - 1);
    Eigen::VectorXd left = x.array().floor();
    Eigen::VectorXd right = x.array().ceil().unaryExpr([widthd](double x) { return std::fmod(x, widthd); });

    // stick into array
    Eigen::Array<int, -1, -1, Eigen::RowMajor> pixelCoords;
    pixelCoords.resize(height*width, 4);
    pixelCoords.col(0) = left.cast<int>();
    pixelCoords.col(1) = right.cast<int>();
    pixelCoords.col(2) = top.cast<int>();
    pixelCoords.col(3) = bot.cast<int>();

    Eigen::VectorXd leftWeight = right - x;
    Eigen::VectorXd rightWeight = x - left;
    Eigen::VectorXd topWeight = bot - y;
    Eigen::VectorXd botWeight = y - top;

    // deals with the case where right == left or top == bot
    leftWeight = (leftWeight.array() == 0).select(1, leftWeight);
    topWeight = (topWeight.array() == 0).select(1, topWeight);

    // stick into array
    Eigen::Array<double, -1, -1, Eigen::RowMajor> weights;
    weights.resize(height*width, 4);
    weights.col(0) = leftWeight;
    weights.col(1) = rightWeight;
    weights.col(2) = topWeight;
    weights.col(3) = botWeight;
    
    return PixelCoords(weights, pixelCoords);
}

EquirectangularImage interpolate(EquirectangularImage original, const Eigen::ArrayXXd &coords){

    const CImg<unsigned char> image = original.getImage();
    const unsigned char* imageBuffer = image.data();
    unsigned char interpolatedImage[original.width*original.height*3];
   
    int height = original.height;
    int width = original.width;
    PixelCoords pixelCoords = calculatePixelCoords(coords, height, width);
    double* pixelWeights = pixelCoords.first.data();
    int* pixelLocations = pixelCoords.second.data();
    
    for (int channel = 0; channel < 3; channel++) {
        int channelOffset = channel*height*width;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int offset = i*width + 4*j;
                int left = pixelLocations[offset + 0];
                int right = pixelLocations[offset + 1];
                int top = pixelLocations[offset + 2];
                int bot = pixelLocations[offset + 3];
                
                unsigned char q00 = imageBuffer[channelOffset + top*width + left];
                unsigned char q01 = imageBuffer[channelOffset + top*width + right];
                unsigned char q10 = imageBuffer[channelOffset + bot*width + left];
                unsigned char q11 = imageBuffer[channelOffset + bot*width + right];
                
                double leftWeight = pixelWeights[offset + 0];
                double rightWeight = pixelWeights[offset + 1];
                double topWeight = pixelWeights[offset + 2];
                double botWeight = pixelWeights[offset + 3];

                double fxTop = leftWeight*q00 + rightWeight*q01;
                double fxBot = leftWeight*q10 + rightWeight*q11;
                double interpolated = botWeight*fxBot + topWeight*fxTop;
                interpolatedImage[channelOffset + i*width + j] = (unsigned char)interpolated;
            }
        }
    }
    CImg<unsigned char> cInterpedImage(interpolatedImage, width, height, 1, 3, true);
    return EquirectangularImage(cInterpedImage);
}

EquirectangularImage rotate(EquirectangularImage image, const Eigen::Matrix3d &rotation){
    EquirectangularSphere sphere(image.height, image.width);
    SphericalCoordinates sphericalCoords = sphere.rotate(rotation);
    return interpolate(image, sphericalCoords.toEquirectangularCoords());
}