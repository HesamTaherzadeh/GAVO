#include "../headers/pose_estimation.h"
#include "../headers/camera.h"
#include "../headers/featurematching.h"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>


static void im(cv::Mat& image){
    cv::Mat temporary;
    cv::resize(image, temporary, cv::Size(), 0.3, 0.2);
    cv::imshow("new", temporary);
    cv::waitKey(0);
}

PoseEstimator::PoseEstimator(Camera camera)
{
    this->camera = camera;
    rotations.push_back(rotation);
    translations.push_back(translation);
    
};

PoseEstimator& PoseEstimator::set_k(Eigen::Matrix3f k){
    this->camera.k = k;
    return *(this);
}

PoseEstimator& PoseEstimator::set_feature(Feature_Matcher feature_arg){
    feature = feature_arg;
    return *this;
};


void PoseEstimator::set_of_points( 
    std::vector<cv::KeyPoint> keypoints_1,
    std::vector<cv::KeyPoint> keypoints_2,
    std::vector<cv::DMatch> matches,
    std::vector<cv::Point2f>& points_query,
    std::vector<cv::Point2f>& points_train
)
    {
        for ( int i = 0; i < ( int ) matches.size(); i++ )
            {
                points_query.push_back ( keypoints_1[matches[i].queryIdx].pt );
                points_train.push_back ( keypoints_2[matches[i].trainIdx].pt );
            }

    };

void PoseEstimator::essential_matrix(
    std::vector<cv::DMatch> matches,
    std::vector<cv::Point2f>& points_query,
    std::vector<cv::Point2f>& points_train,
    cv::Mat& essential_mat)
    
    {
        essential_mat = cv::findEssentialMat ( points_query, points_train, camera.focal_length, camera.principal_point, cv::RANSAC);
    };

std::vector<cv::Point3d> PoseEstimator::triangulate_keypoints(
    cv::Mat& translation_1, 
    cv::Mat& rotation_1,
    cv::Mat& translation_2,
    cv::Mat& rotation_2,
    std::vector<cv::Point2d> points_1,
    std::vector<cv::Point2d> points_2){
    cv::Mat concatenated_Rt_1, concatenated_Rt_2, homogenous_coordinates;
    cv::hconcat(rotation_1, translation_1, concatenated_Rt_1);
    cv::hconcat(rotation_2, translation_2, concatenated_Rt_2);
    cv::triangulatePoints(concatenated_Rt_1, concatenated_Rt_2, points_1, points_2, homogenous_coordinates);
    return this->unhomogenize(homogenous_coordinates);
}

cv::Point2d PoseEstimator::homongenize(
            cv::Point2d point){
            return cv::Point2d
                (
                    ( point.x - (camera.k(0,2)) / (camera.k(0,0))),
                    ( point.y - (camera.k(1,2)) / (camera.k(1,1)))
                );
            };
        
std::vector<cv::Point3d> PoseEstimator::unhomogenize(
            cv::Mat homogenous_points)
            {

                std::vector<cv::Point3d> cartesian_points;
                for(int i; i < homogenous_points.cols; i++){
                    cv::Mat current_point = homogenous_points.col(i);
                    current_point /= current_point.at<float>(3, 0);
                    cartesian_points.push_back(
                        cv::Point3d(current_point.at<float>(0, 0), current_point.at<float>(1, 0), current_point.at<float>(2, 0))
                    );
                };
                return cartesian_points;
            };

void PoseEstimator::recoverpose(
    cv::Mat& frame_one,
    cv::Mat& frame_two,
    cv::Mat& essential_mat,
    cv::Mat& output_feed)
    {
        std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
        std::vector<cv::DMatch> matches;
        std::vector<cv::Point2f> points_query;
        std::vector<cv::Point2f> points_train;
        cv::Mat concatenated_rt;
        feature.match_finder(frame_one, frame_two, keypoints_1, keypoints_2, matches);
        set_of_points( keypoints_1, keypoints_2, matches, points_query, points_train);
        essential_matrix( matches, points_query, points_train, essential_mat);
        cv::drawMatches(frame_one, keypoints_1, frame_two, keypoints_2, matches, output_feed); //  should be embedded in feature 
        cv::recoverPose ( essential_mat, points_query, points_train, rotation, translation, camera.focal_length, camera.principal_point );
        if (count >= 5){
            rotations.erase(rotations.begin());
            translations.erase(translations.begin());
            count = 4;
        };
        count++;
        rotations.push_back(rotations[count-1] * rotation);
        translations.push_back(translation + rotations[count - 1] * translations[count - 1]);
        std::vector<cv::Point2d> points_in_image_1, points_in_image_2;
        for(cv::DMatch match:matches){
            points_in_image_1.push_back(this->homongenize(keypoints_1[match.queryIdx].pt));
            points_in_image_2.push_back(this->homongenize(keypoints_2[match.trainIdx].pt));
        };
        std::vector<cv::Point3d> cartesian_point =  triangulate_keypoints(rotations[count-1], translations[count - 1],
                                                   rotations[count], translations[count], points_in_image_1, points_in_image_2);
        for(cv::Point3d point:cartesian_point){
            std::cout  << point << std::endl;
        }
    };

