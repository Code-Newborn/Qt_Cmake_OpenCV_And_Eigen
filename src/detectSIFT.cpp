#include "detectSIFT.h"

void siftPoint_toshow( std::string path, int numfeature ) {
    cv::Mat img = cv::imread( path );
    cv::imshow( "IMAGE_TO_SHOW", img );

    cv::Ptr< cv::SIFT >         detector = cv::SIFT::create( numfeature );
    std::vector< cv::KeyPoint > keypoints;
    detector->detect( img, keypoints, cv::Mat() );
    std::cout << u8"保留特征点数：" << keypoints.size() << '\n';
    cv::Mat resultImg;
    cv::drawKeypoints( img, keypoints, resultImg, cv::Scalar::all( -1 ), cv::DrawMatchesFlags::DEFAULT );
    cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Display window", resultImg );

    cv::waitKey( 0 );
}