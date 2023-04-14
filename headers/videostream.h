#ifndef video_h
#define video_h

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <opencv2/videoio.hpp>
#include "../headers/featurematching.h"


class VideoStream
{
    
    public:
        VideoStream() = default;
        VideoStream(bool isvideo);
        void showcamera(const char* address, void (*func)(cv::Mat& frame_one,
                                                          cv::Mat& frame_two,
                                                          cv::Mat& essential_mat,
                                                          cv::Mat& feed_out
                                                        ), int wait_key_no);

        void showcamera(void (*func)(cv::Mat& frame_one,
                                     cv::Mat& frame_two,
                                     cv::Mat& essential_mat,
                                     cv::Mat& feed_out
                                     ), int wait_key_no);

        void handle_frames( cv::VideoCapture captured_video,
                            cv::Mat& frame_1, 
                            cv::Mat& frame_2);

    private:
        bool is_it_video {false};
        bool is_it_first_frame {true};
        cv::Mat current_frame;
};


#endif