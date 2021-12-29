#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "sphere.h"

TEST(TestSphereCoordinates, BasicAssertions) {
    int height = 18;
    int width = 36;
    EquirectangularSphere sphere(18, 36);

    // do no rotation
    Eigen::Matrix3d mat = Eigen::Matrix3d::Identity();
    Eigen::ArrayXXd coords =  sphere.rotate(mat).toEquirectangularCoords();

    // expected
    Eigen::VectorXd pixelsWidth, pixelsHeight;
    pixelsWidth = Eigen::VectorXd::LinSpaced(width, 0, width);
    pixelsHeight = Eigen::VectorXd::LinSpaced(height, 0, height);

    int offset;
    double abs_error = 1e-6;
    for (int row = 0; row < height; row++){
        offset = width*row;
        for (int col = 0; col < width; col++){

            ASSERT_NEAR(coords(offset + col, 0), pixelsHeight[row], abs_error);
            if (row == 0){
                continue;
                ASSERT_NEAR(coords(offset + col, 1), 0., abs_error);
            } else {
                ASSERT_NEAR(coords(offset + col, 1), pixelsWidth[col], abs_error);
            }
        }
    }
}


TEST(TestEquirectangularSphere, BasicAssertions) {
    int height = 18;
    int width = 36;
    EquirectangularSphere sphere(18, 36);

    // do no rotation
    Eigen::Matrix3d mat = Eigen::Matrix3d::Identity();
    Eigen::ArrayXXd coords =  sphere.rotate(mat).getCoordinates();

    // expected
    Eigen::VectorXd theta, phi;
    theta = Eigen::VectorXd::LinSpaced(width, 2*M_PI, 0);
    phi = Eigen::VectorXd::LinSpaced(height, 0, M_PI);

    int offset;
    double abs_error = 1e-6;
    for (int row = 0; row < height; row++){
        offset = width*row;
        for (int col = 0; col < width; col++){

            ASSERT_NEAR(coords(offset + col, 0), phi[row], abs_error);
            if (row == 0){
                ASSERT_NEAR(coords(offset + col, 1), 0., abs_error);
            } else {
                ASSERT_NEAR(coords(offset + col, 1), theta[col], abs_error);
                
            }
        }
    }
}

TEST(TestEquirectangularSphereThetaRot, BasicAssertions) {
    int height = 18;
    int width = 36;
    EquirectangularSphere sphere(18, 36);

    // 180 degre rotation
    Eigen::Matrix3d mat;
    mat << -1, 0, 0, 0, -1 ,0, 0, 0, 1;
    Eigen::ArrayXXd coords =  sphere.rotate(mat).getCoordinates();

    // expected theta rotated
    Eigen::VectorXd thetaLeft, thetaRight, phi;
    thetaLeft = Eigen::VectorXd::LinSpaced(width/2, M_PI, 0);
    thetaRight = Eigen::VectorXd::LinSpaced(width/2, 2*M_PI, M_PI);
    phi = Eigen::VectorXd::LinSpaced(height, 0, M_PI);

    int offset;
    double abs_error = 1e-1;
    for (int row = 0; row < height; row++){
        offset = width*row;
        for (int col = 0; col < width; col++){

            ASSERT_NEAR(coords(offset + col, 0), phi[row], abs_error);
            if (row == 0){
                ASSERT_NEAR(coords(offset + col, 1), 0., abs_error);
            } else {
                if (col < width/2){
                    ASSERT_NEAR(coords(offset + col, 1), thetaLeft[col], abs_error);
                } else {
                    ASSERT_NEAR(coords(offset + col, 1), thetaRight[col - width/2], abs_error);
                }
                
            }
        }
    }
}