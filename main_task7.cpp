/*
    * File name: main_task7.cpp
    * Created on : 19th Feb 2024
    * Authors: Ravi Shankar Sankara Narayanan , NUID: 001568628
    * Purpose: This file contains the main function to detect features in a video stream using GoodFeaturesToTrack function.
               This function is based on Shi-Tomasi corner detection algorithm which is based on Harris corner detection algorithm.
*/

#include <opencv2/opencv.hpp>

int main()
{
    cv::VideoCapture cap(0); // Open the default camera

    if (!cap.isOpened())
    { // Check if  succeeded
        return -1;
    }

    while (true)
    {
        cv::Mat frame;
        cap >> frame; // Get a new frame from camera

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY); // Convert to grayscale

        // Detect corners using goodFeaturesToTrack
        std::vector<cv::Point2f> corners;
        cv::goodFeaturesToTrack(gray, corners, 100, 0.1, 1);

        // Draw the corners
        for (size_t i = 0; i < corners.size(); i++)
        {
            cv::circle(frame, corners[i], 5, cv::Scalar(0), 2, 8, 0);
        }

        // Show detected corners
        cv::imshow("Corners", frame);

        if (cv::waitKey(1) >= 0)
            break; // Break the loop if any key is pressed
    }

    // Release the VideoCapture object
    cap.release();

    // Close all the frames
    cv::destroyAllWindows();

    return 0;
}