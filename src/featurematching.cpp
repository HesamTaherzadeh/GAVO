#include "../headers/featurematching.h"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>


void Feature_Matcher::match_finder( const cv::Mat& refrence_frame ,
                                    const cv::Mat& current_frame,
                                    std::vector<cv::KeyPoint>& keypoints_1, std::vector<cv::KeyPoint>& keypoints_2,
                                    std::vector<cv::DMatch>& matches){
    cv::Mat descriptor_current, descriptor_refrence;

    cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
    cv::Ptr<cv::DescriptorExtractor> descriptor = cv::ORB::create();

    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create ( "BruteForce-Hamming" );

    detector->detect (refrence_frame, keypoints_1);
    detector->detect (current_frame, keypoints_2);

    descriptor->compute (refrence_frame, keypoints_1, descriptor_refrence);
    descriptor->compute (current_frame, keypoints_2, descriptor_current);

    std::vector<cv::DMatch> match;
    matcher -> match (descriptor_current, descriptor_refrence, match);

    double min_dist=10000, max_dist=0;

    for (int i{0}; i < descriptor_current.rows; i++ ){
        double temp_dist = match[i].distance;
        if ( temp_dist < min_dist ) min_dist = temp_dist;
        if ( temp_dist > max_dist ) max_dist = temp_dist;
    }

    for ( int i = 0; i < descriptor_current.rows; i++ )
    {
        if ( match[i].distance <= std::max ( 2*min_dist, 30.0 ) )
        {
            matches.push_back ( match[i] );
        }
    }
}

void Feature_Matcher::draw( cv::Mat &image_1, cv::Mat &image_2, std::vector<cv::KeyPoint> keypoint_1,
                            std::vector<cv::KeyPoint> keypoint_2, std::vector<cv::DMatch> matches, cv::Mat output_image, int waitkey,
                            float height_reduction_percent, float width_reduction_percent)
                            {
                            cv::Mat temporary;
                            cv::drawMatches(image_1, keypoint_1, image_2, keypoint_2, matches, temporary);
                            cv::resize(temporary, output_image, cv::Size(), height_reduction_percent, width_reduction_percent);
                            cv::namedWindow( "matches",cv::WINDOW_AUTOSIZE);
                            cv::imshow("matches", output_image);
                            cv::waitKey(waitkey);
                            }
