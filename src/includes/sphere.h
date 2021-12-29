#ifndef SPHERE_H
#define SPHERE_H

#include <Eigen/Dense>


class SphericalCoordinates {
    private:
        Eigen::ArrayXXd coordinates;
        int height, width;
    public:
        SphericalCoordinates(Eigen::ArrayXXd, int, int);
        Eigen::ArrayXXd toEquirectangularCoords();
        Eigen::ArrayXXd getCoordinates();
};


class EquirectangularSphere {
    private:
        int height, width;
    public:
        EquirectangularSphere(int, int);
        EquirectangularSphere();
        SphericalCoordinates rotate(const Eigen::Matrix3d&);
};



#endif