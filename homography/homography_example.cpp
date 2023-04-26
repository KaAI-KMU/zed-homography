#include <vector>
#include <iostream>
#include <opencv2/core/mat.hpp> // mat
#include <opencv2/core/types.hpp> // Point2f
#include <opencv2/calib3d.hpp> // findHomography
#include <opencv2/imgproc.hpp> // warpPerspective
#include <opencv2/highgui.hpp> // imshow, waitKey
#include <opencv2/imgcodecs.hpp> // imread
 
int main( int argc, char** argv)
{
    // Read source image.
    cv::Mat im_src = cv::imread("images/screen_capture.png");
    // Four corners of the book in source image
    std::vector<Point2f> pts_src;
    pts_src.push_back(Point2f(0, 0));
    pts_src.push_back(Point2f(1920, 0));
    pts_src.push_back(Point2f(0, 1200));
    pts_src.push_back(Point2f(1920, 1200));
 
    // Read destination image.
    cv::Mat im_dst = cv::imread("images/zed_img.png");
    // Four corners of the book in destination image.
    std::vector<Point2f> pts_dst;
    pts_dst.push_back(Point2f(506, 228));
    pts_dst.push_back(Point2f(515, 733));
    pts_dst.push_back(Point2f(1415, 166));
    pts_dst.push_back(Point2f(1408, 731));
 
    // Calculate Homography
    cv::Mat h = cv::findHomography(pts_src, pts_dst);
 
    // Output image
    cv::Mat im_out;
    // Warp source image to destination based on homography
    cv::warpPerspective(im_src, im_out, h, im_dst.size());
 
    // Display images
    cv::imshow("Source Image", im_src);
    cv::imshow("Destination Image", im_dst);
    cv::imshow("Warped Source Image", im_out);
 
    waitKey(0);
}