#ifndef CAMERA
#define CAMERA

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <eigen3/Eigen/Core>


struct Camera{
        Eigen::Matrix3f k; 
        cv::Point2d principal_point;
        float focal_length;
        Camera() = default;
        Camera(Eigen::Matrix3f k, cv::Point2d principal_point, float focal_length);
        Camera& set_k(Eigen::Matrix3f k);
        Camera& set_principal_point(cv::Point2d principal_point);
        Camera& set_focal_length(float focal_length);
};

#endif