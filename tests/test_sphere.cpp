#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "sphere.h"


TEST(TestSphereCoordinates, BasicAssertions) {
    int height = 18;
    int width = 36;
    EquirectangularSphere sphere(18, 36);

    // do no rotation
    Eigen::Matrix3d mat = Eigen::Matrix3d::Identity();
    std::vector<double> coords =  sphere.rotate(mat);

    // expected
    Eigen::VectorXd pixelsWidth, pixelsHeight;
    pixelsWidth = Eigen::VectorXd::LinSpaced(width, 0, width);
    pixelsHeight = Eigen::VectorXd::LinSpaced(height, 0, height);

    int offset;
    double abs_error = 2;
    for (int row = 0; row < height; row++){
        for (int col = 0; col < width; col++){

            offset = width*row + col*2;
            ASSERT_NEAR(coords[offset + 0], pixelsHeight[row], abs_error);
            if (row == 0){
                continue;
            } else {
                ASSERT_NEAR(coords[offset + 1], pixelsWidth[col], abs_error);
            }
        }
    }
}

