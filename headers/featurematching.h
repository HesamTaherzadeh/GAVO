#ifndef FEATURE
#define FEATURE

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

class Feature_Matcher{
    
    public:
        Feature_Matcher() = default;
        void match_finder( const cv::Mat& refrence_frame ,
                                    const cv::Mat& current_frame,
                                    std::vector<cv::KeyPoint>& keypoints_1, std::vector<cv::KeyPoint>& keypoints_2,
                                    std::vector<cv::DMatch>& matches);

        void draw( cv::Mat &image_1, cv::Mat &image_2, std::vector<cv::KeyPoint> keypoint_1,
                            std::vector<cv::KeyPoint> keypoint_2, std::vector<cv::DMatch> matches, cv::Mat output_image, int waitkey,
                            float height_reduction_percent, float width_reduction_percent);

};

#endif