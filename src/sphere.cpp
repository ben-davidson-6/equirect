#include "sphere.h"
#include <math.h>
#include <iostream>
#include <vector>

double TWO_PI = 2*M_PI;


SphericalCoordinates::SphericalCoordinates(Eigen::ArrayXXd coordinates, int height, int width): coordinates(coordinates), height(height), width(width) {}

Eigen::ArrayXXd SphericalCoordinates::getCoordinates() {
    return coordinates;
}

Eigen::ArrayXXd SphericalCoordinates::toEquirectangularCoords() {
    double phiToEqui = (1./M_PI)*height;
    double thetaToEqui = (1./TWO_PI)*width;

    Eigen::Array<double, -1, -1, Eigen::RowMajor> equiCoords;
    equiCoords.resize(height*width, 2);
    equiCoords.col(0) = coordinates.col(0)*phiToEqui;
    equiCoords.col(1) = (TWO_PI - coordinates.col(1))*thetaToEqui;
    return equiCoords;
}


EquirectangularSphere::EquirectangularSphere(int height, int width): height(height), width(width) {}
EquirectangularSphere::EquirectangularSphere(): height(180), width(360) {}

SphericalCoordinates EquirectangularSphere::rotate(const Eigen::Matrix3d &rotation){

    Eigen::Vector3d x, rx;
    const Eigen::VectorXd theta = Eigen::VectorXd::LinSpaced(width, TWO_PI, 0);
    const Eigen::VectorXd phi = Eigen::VectorXd::LinSpaced(height, 0, M_PI);
    const Eigen::VectorXd sinTheta = theta.array().sin().matrix();
    const Eigen::VectorXd cosTheta = theta.array().cos().matrix();
    const Eigen::VectorXd sinPhi = phi.array().sin().matrix();
    const Eigen::VectorXd cosPhi = phi.array().cos().matrix();

    double phiR, thetaR;
    std::vector<double> coordBuffer;
    int index, offset;
    for (int row = 0; row < height; row++){
        x[2] = cosPhi(row);

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

            index = offset + 2*col;
            coordBuffer.push_back(phiR);
            coordBuffer.push_back(thetaR);
        }
    }
    Eigen::Map<Eigen::Array<double, -1, -1, Eigen::RowMajor>> coordinates(coordBuffer.data(), height*width, 2);

    return SphericalCoordinates(coordinates, height, width);
}
