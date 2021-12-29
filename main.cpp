#include <Eigen/Dense>
#include "image.h"
#include <iostream>
#include "transformations.cpp"
#include <chrono>

int main() {
    auto start = std::chrono::steady_clock::now();

    EquirectangularImage image("/home/ben/Documents/cpp/equirect/data/equi.png");
    Eigen::Matrix3d rotation;
    rotation << 1, 0, 0, 0, 0 ,-1, 0, 1, 0.;

    Eigen::Matrix3d identity = Eigen::Matrix3d::Identity();
    EquirectangularImage rotated = rotate(image, rotation);
    auto end = std::chrono::steady_clock::now();
    rotated.save("/home/ben/Documents/cpp/equirect/data/equi_saved.jpg");

    std::cout << std::chrono::duration <double, std::milli> (end - start).count() << std::endl;
    return 0;
}