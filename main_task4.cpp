/*
 * File name: main_task4.cpp
 * Created on : 18th Feb 2024
 * Authors: Ravi Shankar Sankara Narayanan , NUID: 001568628
 * Purpose: This files contains the function implemantation for reading the camera parameters and detecting the target and drawing the virtual object on the target.
 */

// Include necessary headers
#include "operations.h"

int main()
{
    // Ask the user for the mode of operation
    std::cout << "Press 'L' for live video projection or any other key for recorded image/video: ";
    char mode;
    std::cin >> mode;

    cv::VideoCapture cap;
    if (mode == 'L' || mode == 'l')
    {
        cap.open(0); // Open the default camera

        // Set the camera resolution
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    }
    else
    {
        // Read the image/video file path
        std::string filePath = "E:/MSCS/CVPR/projects/project4/task4/data/input2.mp4";
        // std::cout << "Enter the path to the image/video file: ";
        // std::cin >> filePath;
        cap.open(filePath); // Open the image/video file
    }

    // Initialize the camera matrix and distortion coefficients
    cv::Mat cameraMatrix, distCoeffs;

    // Read the camera parameters
    readCameraParameters("E:/MSCS/CVPR/projects/project4/task4/src/intrinsic_parameters.xml", cameraMatrix, distCoeffs);
    std::cout << "Camera Matrix has been read successfully " << std::endl;

    while (true)
    {
        cv::Mat frame;
        cap >> frame;

        std::vector<cv::Point2f> corners;
        int key = cv::waitKey(1);

        // Detect the target
        if (detectTarget(frame, corners))
        {

            // Get the pose of the target
            cv::Mat rvec, tvec;
            getBoardPose(corners, cameraMatrix, distCoeffs, rvec, tvec);

            // Check if 'p' is pressed
            if (key == 'p')
            {
                // Print the rotation and translation vectors
                std::cout << "Rotation: " << rvec << "\nTranslation: " << tvec << std::endl;
            }

            // Define the 3D points for the axes
            std::vector<cv::Point3f> axisPoints = {{0, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, 0, -1}}; // Origin, end of X-axis, end of Y-axis, end of Z-axis

            // Project the axes onto the image plane
            std::vector<cv::Point2f> projectedPoints;
            cv::projectPoints(axisPoints, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);

            // Draw the axes on the frame
            cv::line(frame, projectedPoints[0], projectedPoints[1], cv::Scalar(255, 0, 0), 2); // X-axis in red
            cv::line(frame, projectedPoints[0], projectedPoints[2], cv::Scalar(0, 255, 0), 2); // Y-axis in green
            cv::line(frame, projectedPoints[0], projectedPoints[3], cv::Scalar(0, 0, 255), 2); // Z-axis in blue

            // Draw the virtual object
            // drawPyramid(frame, rvec, tvec, cameraMatrix, distCoeffs);

            // Draw the virtual object
            // drawCube(frame, rvec, tvec, cameraMatrix, distCoeffs);

            drawHouse(frame, rvec, tvec, cameraMatrix, distCoeffs);

            // drawDiamond(frame, rvec, tvec, cameraMatrix, distCoeffs);
        }

        // Display the frame
        cv::imshow("Live Feed", frame);

        // Check if 'q' is pressed or the image/video file has ended
        if (mode != 'L' && mode != 'l' && frame.empty())
            break;
        else if (cv::waitKey(1) == 'q')
            break;
    }
    return 0;
}