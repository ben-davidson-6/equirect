#ifndef SPHERE_H
#define SPHERE_H

#include <Eigen/Dense>
#include <vector>

typedef Eigen::Array<double, -1, -1, Eigen::RowMajor> ArrayDRow;
typedef Eigen::Map<ArrayDRow> MapArrayD;

class EquirectangularSphere {
    private:
        int height, width;
    public:
        EquirectangularSphere(int, int);
        std::vector<double> rotate(const Eigen::Matrix3d&);
};



#endif