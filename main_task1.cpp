/*
 * File name: main_task1.cpp
 * Created on : 17th Feb 2024
 * Authors: Ravi Shankar Sankara Narayanan , NUID: 001568628
 * Purpose: This file is the main file that runs the camera calibration system and saves the parameters.
 */

// Include the necessary header files
#include <opencv2/opencv.hpp>
#include "operations.h"

int main()
{
    // Open the default camera
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cout << "Error opening video stream" << std::endl;
        return -1;
    }

    // Set the frame width and height
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 854);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    // Create vectors to store the corners and the corresponding 3D world points
    std::vector<std::vector<cv::Point2f>> corner_list;
    std::vector<std::vector<cv::Vec3f>> point_list;

    // Print the instructions
    std::cout << "Press 'd' to display the number of corners and the coordinates of the first corner\n";
    std::cout << "Press 's' to store the corners and the corresponding 3D world points\n";
    std::cout << "Press 'q' to exit\n";
    std::cout << "Please select upto 5 frames for calibration\n";

    cv::Mat lastValidFrame;
    while (true)
    {
        cv::Mat frame;
        cap >> frame; // Capture frame-by-frame

        if (frame.empty())
        {
            break; // If the frame is empty, break immediately
        }

        lastValidFrame = frame.clone();

        int key = cv::waitKey(1);

        // Call the function to find and draw the corners
        findChessboardCorners(frame, corner_list, point_list, key);

        // Display the resulting frame
        cv::imshow("Frame", frame);

        // Press 'q' on the keyboard to exit
        if (key == 'q')
        {
            break;
        }
    }

    // When everything is done, release the video capture object
    cap.release();

    // Close all the frames
    cv::destroyAllWindows();

    // Call the calibration function
    runCalibration(corner_list, point_list, lastValidFrame);
    /*
    for (size_t i = 0; i < corner_list.size(); i++)
        {
            std::cout << "Frame " << i + 1 << ":\n";
            for (size_t j = 0; j < corner_list[i].size(); j++)
            {
                std::cout << "Corner " << j + 1 << ": (" << corner_list[i][j].x << ", " << corner_list[i][j].y << ")\n";
                std::cout << "Point " << j + 1 << ": (" << point_list[i][j][0] << ", " << point_list[i][j][1] << ", " << point_list[i][j][2] << ")\n";
            }
            std::cout << "\n";
        }
    */
    return 0;
}