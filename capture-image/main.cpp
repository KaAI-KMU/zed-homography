#include <sl/Camera.hpp>
#include <filesystem>

int main(int argc, char **argv) {

    // Create a ZED camera object
    sl::Camera zed;

    // Set configuration parameters
    sl::InitParameters init_parameters;
    init_parameters.camera_resolution = sl::RESOLUTION::HD1080; // Use HD720 opr HD1200 video mode, depending on camera type.
    init_parameters.camera_fps = 30; // Set fps at 30

    // 현재 실행 파일의 경로를 얻어옴
    std::string exe_path = std::filesystem::path(argv[0]).parent_path();
    std::string directory = (exe_path + "/images/");
    std::filesystem::create_directories(directory);

    // Open the camera
    auto returned_state = zed.open(init_parameters);
    if (returned_state != sl::ERROR_CODE::SUCCESS) {
        std::cout << "Error " << returned_state << ", exit program." << std::endl;
        return EXIT_FAILURE;
    }

    // Capture 50 frames and stop
    int i = 0;
    sl::Mat zed_image;
    while (i < 50) {
        // Grab an image
        returned_state = zed.grab();
        // A new image is available if grab() returns ERROR_CODE::SUCCESS
        if (returned_state == sl::ERROR_CODE::SUCCESS) {

            // Get the left image
            zed.retrieveImage(zed_image, sl::VIEW::LEFT);
            
            // Display the image resolution and its acquisition timestamp
            std::cout<<"Image resolution: "<< zed_image.getWidth()<<"x"
                      <<zed_image.getHeight() <<" || Image timestamp: "
                      <<zed_image.timestamp.data_ns<<std::endl;

            // Make filename format "iamge{i}.png"
            std::string filename = "image" + std::to_string(i) +".png";

            // Save image in build/images/
            zed_image.write((directory + filename).c_str());
            i++;
        }
    }

    // Close the camera
    zed.close();
    return EXIT_SUCCESS;
}

