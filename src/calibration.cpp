#include "../headers/camera.h"
#include "../headers/calibration.h"


Calibration::Calibration(std::vector<cv::Mat> vector_of_images){
    this->vector_of_images = vector_of_images;
};