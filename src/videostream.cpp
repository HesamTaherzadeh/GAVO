#include "../headers/videostream.h"
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <ctime>
#include "../headers/featurematching.h"



VideoStream::VideoStream(bool isvideo){
    /*
    * class to conduct video stream either from a video or camera 
    * @param isvideo : boolean to show wether if the stream comes from a video or camera
    */
    is_it_video = isvideo;
};

void VideoStream::handle_frames(cv::VideoCapture captured_video,
                                cv::Mat& frame_1, 
                                cv::Mat& frame_2)
{
    if(!is_it_first_frame){
        frame_1 = current_frame;
        captured_video >> frame_2;
        current_frame = frame_2;
    }
    else{
        captured_video >> frame_1;
        std::cout << "Initializing :" << std::endl;
        std::cout << "Slowly translate the camera, avoid pure rotation to initialize and whenever you were ready press a key..." << std::endl;
        cv::waitKey(0);
        captured_video >> frame_2;
        current_frame = frame_2;
        is_it_first_frame = false;
    }
}

void VideoStream::showcamera(const char* address, void (*func)(
                                             cv::Mat& frame_one,
                                             cv::Mat& frame_two,
                                             cv::Mat& essential_mat,
                                             cv::Mat& feed_out
                                            ), int wait_key_no)
{
    /*
    * method that recieves an address for a video and applies a function on it and also writes the FPS over it 
    * @param address: the address for the video 
    * @param func : the function to be applied on the recieving frames 
    * @param wait_key_no : the argument which will be passed to cv::waitKey
    */
    cv::VideoCapture captured_video(address);
    cv::Mat essential_mat;
    captured_video.set(cv::CAP_PROP_BUFFERSIZE, 3);
    captured_video.set(cv::CAP_PROP_FPS, 60);
    clock_t prev_frame_time, compute_time;
    if(!captured_video.isOpened())
    {
        std::cout << "The stream either has ended or faced a problem" << std::endl;
    }

    while (1)
    {      
        cv::Mat frame_one, frame_two, frame; // two consecutive frames obtained from camera feed, and the output feed
        this->handle_frames(captured_video, frame_one, frame_two);
        if(frame_one.empty() || frame_two.empty() )
            break;
        prev_frame_time = clock(); // start of timing for showing FPS 
        func(frame_one, frame_two, essential_mat, frame);
        compute_time = clock() - prev_frame_time;
        int fps = CLOCKS_PER_SEC / compute_time;
        cv::putText(frame, 
            "FPS : " + std::to_string(fps),
            cv::Point(10, frame.rows / 15), 
            cv::FONT_HERSHEY_DUPLEX,
            1.0,
            CV_RGB(0, 0, 255), //font color
            2);    
        cv::imshow("Frame", frame);
        char c=(char)cv::waitKey(wait_key_no);
        if(c==27)
            break;   
    };
    captured_video.release();
    cv::destroyAllWindows();
};


void VideoStream::showcamera(void (*func)(cv::Mat& frame_one,
                                             cv::Mat& frame_two,
                                             cv::Mat& essential_mat,
                                             cv::Mat& feed_out
                                            ), int wait_key_no){
    /*
    * Overloads pervious method , just for camera streams
    * method that recieves an address for a video and applies a function on it and also writes the FPS over it 
    * @param address: the address for the video 
    * @param func : the function to be applied on the recieving frames 
    * @param wait_key_no : the argument which will be passed to cv::waitKey
    */

    cv::Mat essential_mat;
    cv::VideoCapture captured_video(0);
    // captured_video.set(cv::CAP_PROP_BUFFERSIZE, 3); // setting buffersize 
    // captured_video.set(cv::CAP_PROP_FPS, 60); // setting proper FP S
    clock_t prev_frame_time, compute_time;
    if(!captured_video.isOpened())
    {
        std::cout << "The stream either has ended or faced a problem" << std::endl;
    }

    while (1)
    {      
        cv::Mat frame_one, frame_two, frame; // two consecutive frames obtained from camera feed, and the output feed
        this->handle_frames(captured_video, frame_one, frame_two);
        if(frame_one.empty() || frame_two.empty() )
            break;
        prev_frame_time = clock(); // start of timing for showing FPS 
        func(frame_one, frame_two, essential_mat, frame); // the actual recover pose being applied 
        compute_time = clock() - prev_frame_time; // end of process time, for showing FPS
        int fps = CLOCKS_PER_SEC / compute_time; // compute of FPS 
        cv::putText(frame, 
            "FPS : " + std::to_string(fps),
            cv::Point(10, frame.rows / 15), 
            cv::FONT_HERSHEY_DUPLEX,
            1.0,
            CV_RGB(0, 0, 255), //font color
            2);    
        cv::imshow("Frame", frame);
        char c=(char)cv::waitKey(wait_key_no);
        if(c==27)
            break;   
    };
    captured_video.release();
    cv::destroyAllWindows();

};
