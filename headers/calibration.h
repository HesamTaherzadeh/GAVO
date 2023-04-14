#ifndef CALIBRATION
#define CALIBRATION

#include "../headers/camera.h"
#include <vector>



class Calibration: public Camera{
    public:
        Calibration() = default;
        Calibration(std::vector<cv::Mat> vector_of_images);
    private:
        std::vector<cv::Mat> vector_of_images;

};



#endif