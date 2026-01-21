#include <iostream>
#include <process.h>
#include <windows.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Dense"
#include "Eigenvalues"

#include "detectSIFT.h"
#include "Rotmat2Euler.h"
#include "detectAruco.h"

int main() {
    SetConsoleOutputCP( CP_UTF8 );  // 设置控制台输出为UTF-8
    SetConsoleCP( CP_UTF8 );        // 设置控制台输入为UTF-8

    siftPoint_toshow( "..\\..\\lena.jpg", 400 );  // SIFT特征点检测，相对exe路径
    cv::waitKey( 0 );

    detectAruco( "..\\..\\Camera1.png", 22 );  // 识别Aruco码，相对exe路径
    cv::waitKey( 0 );

    Eigen::Matrix3f rotation_mat;
    Eigen::Vector3f translation_mat( -93.8982f, -10.6360f, -28.6675f );
    Eigen::Vector3f euler_rad;
    rotation_mat << 0.9941f, 0.1086f, -0.0071f, -0.1085f, 0.9940f, 0.0136f, 0.0085f, -0.0127f, 0.9999f;

    euler_rad = Mat_Rzyx( rotation_mat );
    std::cout << "Rotation Mat" << std::endl;
    std::cout << rotation_mat << std::endl;  // 打印Eigen矩阵
    std::cout << "Euler Angle in Radian ZYX(User)" << std::endl;
    std::cout << euler_rad << std::endl;

    Eigen::Matrix3d rotation_mat_eigen;
    Eigen::Vector3d euler_rad_eigen;
    rotation_mat_eigen << -0.02697f, -0.96295f, 0.268318f, -0.98587f, 0.07001f, 0.15214f, -0.16529f, -0.26042f, -0.95123f;
    euler_rad_eigen = rotation_mat_eigen.eulerAngles( 2, 1, 0 );
    std::cout << "Euler Angle in Radian ZYX(Eigen)" << std::endl;
    std::cout << euler_rad_eigen << std::endl;

    cv::Mat_< double > Mat1 = ( cv::Mat_< double >( 4, 4 ) << 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6 );

    std::cout << std::endl;
    std::cout << "Origin Mat" << std::endl << Mat1 << std::endl;
    cv::Mat Mat1_slice = Mat1( cv::Rect( 3, 0, 1, 3 ) );
    std::cout << "Mat Slice" << std::endl << Mat1_slice << std::endl;

    cv::Mat_< double > Mat2 = ( cv::Mat_< double >( 3, 3 ) << -0.02697f, -0.96295f, 0.268318f, -0.98587f, 0.07001f, 0.15214f, -0.16529f, -0.26042f, -0.95123f );
    cv::Mat_< double > Mat2_inv;
    cv::invert( Mat2, Mat2_inv );
    std::cout << "Mat Inverse" << std::endl << Mat2_inv << std::endl;
    std::cout << "Mat_origin * Mat_inv" << std::endl << Mat2 * Mat2_inv << std::endl;

    // note 罗德里格斯转换 旋转向量转旋转矩阵
    cv::Mat_< double > Rot_Vec = ( cv::Mat_< double >( 3, 1 ) << -0.1342, -0.0318, -0.4398 );
    cv::Mat            Rot_Mat;
    cv::Rodrigues( Rot_Vec, Rot_Mat );
    std::cout << "Rot_Vec To Rot_Mat" << std::endl << Rot_Mat << std::endl;

    std::cout << "\n程序运行结束，按回车键退出...";
    std::cin.ignore( std::numeric_limits< std::streamsize >::max(), '\n' );  // 清空缓冲
    std::cin.get();                                                          // 等待用户按回车

    system( "pause" );

    return 0;
}
