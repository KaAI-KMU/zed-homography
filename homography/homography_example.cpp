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
    std::string imageDir = "/home/kaai5/zed-homography/homography/images";
    // Read source image.
    cv::Mat im_src = cv::imread(imageDir + "/screen_capture.png", 1);
    if (im_src.empty()) {
        std::cout << "no source image" << std::endl;
        return 0;
    }

    // Four corners of monitor in source image
    std::vector<cv::Point2f> pts_src;
    pts_src.push_back(cv::Point2f(0, 0));
    pts_src.push_back(cv::Point2f(1920, 0));
    pts_src.push_back(cv::Point2f(0, 1200));
    pts_src.push_back(cv::Point2f(1920, 1200));
 
    // Read destination image.
    cv::Mat im_dst = cv::imread(imageDir + "/zed_img.png", 1);
    if (im_dst.empty()) {
        std::cout << "no destination image" << std::endl;
        return 0;
    }
    // Four corners of zed in destination image.
    std::vector<cv::Point2f> pts_dst;
    pts_dst.push_back(cv::Point2f(506, 175));
    pts_dst.push_back(cv::Point2f(1423,160));
    pts_dst.push_back(cv::Point2f(520, 737));
    pts_dst.push_back(cv::Point2f(1408, 731));
 
    // Calculate Homography matrix h
    cv::Mat h = cv::findHomography(pts_src, pts_dst);
 
    // Output image
    cv::Mat im_out;
    // Warp source image to destination based on homography
    cv::warpPerspective(im_src, im_out, h, im_dst.size());

    // Define a vector of Point2f to store the transformed points
    std::vector<cv::Point2f> transformed_pts;
    // Transform the source points to destination using the homography matrix
    cv::perspectiveTransform(pts_src, transformed_pts, h);

    // Print the transformed points
    std::cout << "Transformed points: " << std::endl;
    for (const auto & i : transformed_pts)
        std::cout << "Point " << i.x << ", " << i.y << std::endl;
 
    // Display images
//    cv::imshow("Source Image", im_src);
//    cv::imshow("Destination Image", im_dst);
//    cv::imshow("Warped Source Image", im_out);

    // Transform a given point from the source image to the destination image
    cv::Point2f pt_src(500, 1002);
    cv::Mat src_pt_mat = cv::Mat::zeros(1, 1, CV_32FC2);
    src_pt_mat.at<cv::Point2f>(0) = pt_src;
    cv::Mat dst_pt_mat;
    cv::perspectiveTransform(src_pt_mat, dst_pt_mat, h);
    cv::Point2f pt_dst = dst_pt_mat.at<cv::Point2f>(0);

    // Draw red dot on source image at the given point
    cv::circle(im_src, pt_src, 5, cv::Scalar(0, 0, 255), -1);

    // Draw red dot on destination image at the transformed point
    cv::circle(im_dst, pt_dst, 5, cv::Scalar(0, 0, 255), -1);

    // Display images with red dot
    cv::Mat combined;
    // Resize images to the same size if needed
    if (im_src.size() != im_dst.size()) {
        cv::resize(im_src, im_src, im_dst.size());
    }
    hconcat(im_src, im_dst, combined);
    cv::imshow("Transformed Gaze point", combined);
 
    cv::waitKey(0);
}
