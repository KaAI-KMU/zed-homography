#include <vector>
#include <iostream>
#include <random>
#include <opencv2/core/mat.hpp> // mat
#include <opencv2/core/types.hpp> // Point2f
#include <opencv2/calib3d.hpp> // findHomography
#include <opencv2/imgproc.hpp> // warpPerspective
#include <opencv2/highgui.hpp> // imshow, waitKey
#include <opencv2/imgcodecs.hpp> // imread

void GetRandomCoord(std::vector<std::pair<int, int>>& vec, int dot_num)
{
    std::random_device rd;

    // random_device 를 통해 난수 생성 엔진을 초기화 한다.
    std::mt19937 gen(rd());

    // 0 부터 1920, 1080 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
    std::uniform_int_distribution<int> source_x(0, 1920);
    std::uniform_int_distribution<int> source_y(0, 1080);

    for (int i = 0; i < dot_num; i++) {
        std::cout << "x: " << source_x(gen) << " y: " << source_y(gen) << std::endl;
        vec.push_back(std::make_pair(source_x(gen), source_y(gen)));
    }
}

cv::Scalar GetRandomColor() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> r(0, 255);
    std::uniform_int_distribution<int> g(0, 255);
    std::uniform_int_distribution<int> b(0, 255);

    return cv::Scalar(r(gen), g(gen), b(gen));
}


int main( int argc, char** argv)
{
    std::string imageDir = "/home/kaai5/zed-homography/homography/images";
    // Read source image.
    cv::Mat im_src = cv::imread(imageDir + "/source.png", 1);
    if (im_src.empty()) {
        std::cout << "no source image" << std::endl;
        return 0;
    }

    // Four corners of monitor in source image
    std::vector<cv::Point2f> pts_src;
    pts_src.push_back(cv::Point2f(0, 0));
    pts_src.push_back(cv::Point2f(1920, 0));
    pts_src.push_back(cv::Point2f(0, 1080));
    pts_src.push_back(cv::Point2f(1920, 1080));
    pts_src.push_back(cv::Point2f(960, 1080));
    pts_src.push_back(cv::Point2f(960, 0));
 
    // Read destination image.
    cv::Mat im_dst = cv::imread(imageDir + "/destination.png", 1);
    if (im_dst.empty()) {
        std::cout << "no destination image" << std::endl;
        return 0;
    }
    // Four corners of zed in destination image.
    std::vector<cv::Point2f> pts_dst;
    pts_dst.push_back(cv::Point2f(306, 423));
    pts_dst.push_back(cv::Point2f(1251, 427));
    pts_dst.push_back(cv::Point2f(322, 943));
    pts_dst.push_back(cv::Point2f(1241, 948));
    pts_dst.push_back(cv::Point2f(796, 936));
    pts_dst.push_back(cv::Point2f(775, 423));

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
    cv::imshow("Source Image", im_src);
    cv::imshow("Destination Image", im_dst);
    cv::imshow("Warped Source Image", im_out);

    // Display images with red dot
    cv::Mat combined;
    // Resize images to the same size if needed
    if (im_src.size() != im_dst.size()) {
        cv::resize(im_src, im_src, im_dst.size());
    }

    // Combine two images
    hconcat(im_src, im_dst, combined);

    // Generate random coordinates
    std::vector<std::pair<int, int>> random_coord;
    GetRandomCoord(random_coord, 20);

    for (int i = 0; i < random_coord.size(); i++) {
        // Transform a given point from the source image to the destination image
        cv::Point2f pt_src(random_coord[i].first, random_coord[i].second);
        cv::Mat src_pt_mat = cv::Mat::zeros(1, 1, CV_32FC2);
        src_pt_mat.at<cv::Point2f>(0) = pt_src;
        cv::Mat dst_pt_mat;
        cv::perspectiveTransform(src_pt_mat, dst_pt_mat, h);
        cv::Point2f pt_dst = dst_pt_mat.at<cv::Point2f>(0);

        // Define random color
        cv::Scalar color = GetRandomColor();

        // Draw red dot on source image at the given point
        cv::circle(im_src, pt_src, 5, color, -1);

        // Draw red dot on destination image at the transformed point
        cv::circle(im_dst, pt_dst, 5, color, -1);

        pt_dst.x += im_src.cols;
        // Draw the line on the combined image
        cv::line(combined, pt_src, pt_dst, color, 2);
    }
    cv::imshow("Transformed Gaze point", combined);
 
    cv::waitKey(0);
}
