#include "../headers/camera.h"

Camera::Camera(Eigen::Matrix3f k, cv::Point2d principal_point, float focal_length){
    /*
    * An struct to handle camera and its parameters
    * TODO: Consider adding a calibration method 
    * @param k : The intrisincs matrix of camera
    * @param principal_point : the principal point of the camera (offset from the center of image plane)
    * @param focal_length: the focal length of the camera 
    */
    this -> k = k;
    this -> principal_point = principal_point;
    this -> focal_length = focal_length;
}

Camera& Camera::set_k(Eigen::Matrix3f k){
    /*
    * The method to set the k 
    * ! deprecated --> changed from class to struct
    */
    this -> k = k;
    return *this;
}

Camera& Camera::set_principal_point(cv::Point2d principal_point){
    /*
    * The method to set the principal point 
    * ! deprecated --> changed from class to struct
    */
    this -> principal_point = principal_point;
    return *this;
}

Camera& Camera::set_focal_length(float focal_length){
    /*
    * The method to set the focal length 
    * ! deprecated --> changed from class to struct
    */
    this -> focal_length = focal_length;
    return *this;
}