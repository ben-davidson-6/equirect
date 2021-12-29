#include <Eigen/Dense>
#include "image.h"
#include <iostream>
#include "transformations.cpp"

int main() {
    EquirectangularImage image("/home/ben/Documents/cpp/equirect/data/equi.png");
    Eigen::Matrix3d rotation;
    rotation << 1, 0, 0, 0, 0 ,-1, 0, 1, 0.;

    Eigen::Matrix3d identity = Eigen::Matrix3d::Identity();
    EquirectangularImage rotated = rotate(image, rotation);
    rotated.save("/home/ben/Documents/cpp/equirect/data/equi_saved.png");
    return 0;
}