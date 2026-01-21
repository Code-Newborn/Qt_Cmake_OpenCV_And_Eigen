#include "Dense"

#define PI 3.14159265359

Eigen::Vector3f Mat_Rxyz(Eigen::Matrix3f RotationMat);
Eigen::Vector3f Mat_Ryzx(Eigen::Matrix3f RotationMat);
Eigen::Vector3f Mat_Rzxy(Eigen::Matrix3f RotationMat);

Eigen::Vector3f Mat_Rzyx(Eigen::Matrix3f RotationMat);
Eigen::Vector3f Mat_Ryxz(Eigen::Matrix3f RotationMat);
Eigen::Vector3f Mat_Rxzy(Eigen::Matrix3f RotationMat);