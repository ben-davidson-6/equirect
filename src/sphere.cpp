#include "sphere.h"
#include <math.h>
#include <iostream>

double TWO_PI = 2*M_PI;


EquirectangularSphere::EquirectangularSphere(int height, int width): height(height), width(width) {}


std::vector<double> EquirectangularSphere::rotate(const Eigen::Matrix3d &rotation){

    Eigen::Vector3d x, rx;
    const Eigen::VectorXd theta = Eigen::VectorXd::LinSpaced(width, TWO_PI, 0);
    const Eigen::VectorXd phi = Eigen::VectorXd::LinSpaced(height, 0, M_PI);
    const Eigen::VectorXd sinTheta = theta.array().sin();
    const Eigen::VectorXd cosTheta = theta.array().cos();
    const Eigen::VectorXd sinPhi = phi.array().sin();
    const Eigen::VectorXd cosPhi = phi.array().cos();

    double phiR, thetaR;
    double phiToEqui = (1./M_PI)*height;
    double thetaToEqui = (1./TWO_PI)*width;
    std::vector<double> coordBuffer;
    double widthD = width;

    for (int row = 0; row < height; row++){

        x[2] = cosPhi[row];
        for (int col = 0; col < width; col++) {

            // project to r3
            x[0] = cosTheta[col]*sinPhi[row];
            x[1] = sinTheta[col]*sinPhi[row];

            // rotate
            rx = rotation*x;

            // return to sphere
            phiR = std::acos(rx[2]);
            thetaR = std::atan2(rx[1], rx[0]);
            thetaR = (thetaR < 0) ? thetaR + TWO_PI: thetaR;

            // add coordinates
            coordBuffer.push_back(phiR*phiToEqui);
            coordBuffer.push_back(std::fmod(((TWO_PI - thetaR)*thetaToEqui), widthD));
       }
    }
    return coordBuffer;
}
