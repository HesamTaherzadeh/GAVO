#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "headers/featurematching.h"
#include "headers/camera.h"
#include "headers/pose_estimation.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "headers/videostream.h"
#include <eigen3/Eigen/Core>
#include <vector>
#include <queue>


Feature_Matcher feature;
VideoStream videostream;
std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
std::vector<cv::DMatch> matches;
Eigen::Matrix3f k;
cv::Point2d principal_point( -0.0610, -0.1611 );
cv::Mat essential_mat, rotation, translation, concatenated;
float focal_length {22.5502};
Camera camera(k, principal_point, focal_length);
PoseEstimator pose_estimation(camera);

void wrap_func( cv::Mat& frame_one,
                cv::Mat& frame_two,
                cv::Mat& essential_mat,
                cv::Mat& frame){
                camera.set_k(k);
                pose_estimation.recoverpose(frame_one,
                                            frame_two,
                                            essential_mat,
                                            frame);
                                };


cv::Mat webcam(cv::Mat& vid){
    return vid;
}
void im(cv::Mat& image){
    cv::Mat temporary;
    cv::resize(image, temporary, cv::Size(), 0.3, 0.2);
    cv::imshow("new", temporary);
    cv::waitKey(0);
}
int main(int argc, char* argv[]){
    k << 22.5502, 0, -0.0610, 0, 22.5502, -0.1611, 0, 0, 1 ;
    pose_estimation.set_k(k);
    pose_estimation.set_feature(feature);
    videostream.showcamera(*(wrap_func), 1);
}