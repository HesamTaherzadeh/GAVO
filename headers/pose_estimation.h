#ifndef POSE
#define POSE

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <eigen3/Eigen/Core>
#include "../headers/camera.h"
#include "../headers/featurematching.h"
#include <vector>


class PoseEstimator{
    public:
        PoseEstimator() = default;
        PoseEstimator(Camera camera);
        void recoverpose(
            cv::Mat& frame_one,
            cv::Mat& frame_two,
            cv::Mat& essential_matrix,
            cv::Mat& output_feed);
        PoseEstimator& set_feature(Feature_Matcher feature_arg);

        std::vector<cv::Mat> rotations;
        std::vector<cv::Mat> translations;
        cv::Mat translation = (cv::Mat_<double>(3,1)<< 0, 0, 0);
        cv::Mat rotation = (cv::Mat_<double>(3,3) << 1, 0, 0, 0, 1, 0, 0, 0, 1);
        PoseEstimator& set_k(Eigen::Matrix3f k);
        int count = 0;



    private:
        Camera camera;
        Feature_Matcher feature;
        void essential_matrix(
                            std::vector<cv::DMatch> matches,
                            std::vector<cv::Point2f>& points_query,
                            std::vector<cv::Point2f>& points_train,
                            cv::Mat& essential_mat);

        void essential_matrix( 
                            std::vector<cv::Point2f> points_query,
                            std::vector<cv::Point2f> points_train,
                            std::vector<cv::DMatch > matches,
                            cv::Mat& essential_matrix);
    
        void set_of_points( 
                            std::vector<cv::KeyPoint> keypoints_1,
                            std::vector<cv::KeyPoint> keypoints_2,
                            std::vector<cv::DMatch> matches,
                            std::vector<cv::Point2f>& points_query,
                            std::vector<cv::Point2f>& points_train);
        
        std::vector<cv::Point3d> triangulate_keypoints(
                            cv::Mat& translation_1, 
                            cv::Mat& rotation_1,
                            cv::Mat& translation_2,
                            cv::Mat& rotation_2,
                            std::vector<cv::Point2d> points_1,
                            std::vector<cv::Point2d> points_2);
        
        cv::Point2d homongenize(
                            cv::Point2d point);
        
        std::vector<cv::Point3d> unhomogenize(
                            cv::Mat homogenous_points);

};    

#endif