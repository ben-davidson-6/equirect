#include <Eigen/Dense>
#include "image.h"
#include <math.h>
#include "sphere.h"
#include <iostream>

typedef Eigen::Array<int, -1, -1, Eigen::RowMajor> ArrayIRow ;
typedef Eigen::Array<double, -1, -1, Eigen::RowMajor> ArrayDRow;
typedef std::pair<ArrayDRow, ArrayIRow> PixelCoords;


void calculatePixelCoords(const std::vector<double> &equiCoords, std::vector<int> &pixelLocations, std::vector<double> &pixelWeights, int height, int width) {
    double maxY = height - 1;
    double maxX = width;
    for (size_t i = 0; i < height; i++)
    {
        int rowOffset = i*width*2;

        for (size_t j = 0; j < width; j++)
        {
            int colOffset = j*2;
            double x = equiCoords[rowOffset + colOffset + 1];
            double y = equiCoords[rowOffset + colOffset];

            double top = std::floor(y);
            double bot = std::min(maxY, std::ceil(y));
            double left = std::floor(x);
            double right = std::fmod(std::ceil(x), maxX);
            
            double leftWeight = right - x;
            double rightWeight = x - left;
            double topWeight = bot - y;
            double botWeight = y - top;

            leftWeight = leftWeight == 0 ? 1 : leftWeight;
            topWeight = topWeight == 0 ? 1 : topWeight;

            pixelWeights.push_back(leftWeight);
            pixelWeights.push_back(rightWeight);
            pixelWeights.push_back(topWeight);
            pixelWeights.push_back(botWeight);

            pixelLocations.push_back(left);
            pixelLocations.push_back(right);
            pixelLocations.push_back(top);
            pixelLocations.push_back(bot);
        }
        
    }
}


EquirectangularImage interpolate(EquirectangularImage original, const std::vector<double> &equiCoords){

    const CImg<unsigned char> image = original.getImage();
    const unsigned char* imageBuffer = image.data();
    unsigned char interpolatedImage[original.width*original.height*3];
   
    int height = original.height;
    int width = original.width;
    std::vector<int> pixelLocations;
    std::vector<double> pixelWeights;
    calculatePixelCoords(equiCoords, pixelLocations, pixelWeights, height, width);


    for (int channel = 0; channel < 3; channel++) {
        int channelOffset = channel*height*width;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int offset = i*width*4 + 4*j;
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
    std::vector<double> equiCoords = sphere.rotate(rotation);
    return interpolate(image, equiCoords);
}