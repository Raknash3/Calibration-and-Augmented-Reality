/*
    * File name: operations_task1.cpp
    * Created on : 17th Feb 2024
    * Authors: Ravi Shankar Sankara Narayanan , NUID: 001568628
    * Purpose: This files contains the definitions of the functions implemented in the main_task1.cpp file. The functions include findChessboardCorners and runCalibration.
               They perform the camera calibration and save the parameters.
*/

// Include the necessary header files
#include "operations.h"
#include <chrono>
#include <iostream>

// Function to find and draw the corners and store the corresponding 3D world points
void findChessboardCorners(cv::Mat &frame, std::vector<std::vector<cv::Point2f>> &corner_list, std::vector<std::vector<cv::Vec3f>> &point_list, int key)
{

    static int imageCounter = 0;

    // Define the size of the checkerboard
    cv::Size patternSize(9, 6); // Change this according to your checkerboard

    // Vector to store the corners
    std::vector<cv::Point2f> corners;

    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Find the corners
    bool patternFound = cv::findChessboardCorners(gray, patternSize, corners);

    if (patternFound)
    {
        // Refine the corners
        cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
                         cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));

        // Draw the corners
        cv::drawChessboardCorners(frame, patternSize, cv::Mat(corners), patternFound);

        // Check if 'd' is pressed
        if (key == 'd')
        {
            // Print the number of corners and the coordinates of the first corner
            std::cout << "Number of corners: " << corners.size() << std::endl;
            std::cout << "First corner: (" << corners[0].x << ", " << corners[0].y << ")" << std::endl;
        }

        // Check if 's' is pressed
        if (key == 's')
        {
            // Store the corners in corner_list
            corner_list.push_back(corners);
            std::cout << "Corner list size: " << corner_list.size() << std::endl;

            // Create a point_set for the corresponding 3D world points
            std::vector<cv::Vec3f> point_set;
            for (int i = 0; i < patternSize.height; ++i)
            {
                for (int j = 0; j < patternSize.width; ++j)
                {
                    point_set.push_back(cv::Vec3f(j, i, 0));
                }
            }

            // Store the point_set in point_list
            point_list.push_back(point_set);
            std::cout << "Point list size: " << point_list.size() << std::endl;

            // Save the current frame
            std::string filename = "E:/MSCS/CVPR/projects/project4/task1/data/calibration/calibration_image_" + std::to_string(imageCounter) + ".jpg";
            cv::imwrite(filename, frame);
            imageCounter++;
        }
    }
}

// Function to run the calibration and save the parameters
void runCalibration(std::vector<std::vector<cv::Point2f>> &corner_list, std::vector<std::vector<cv::Vec3f>> &point_list, cv::Mat &frame)
{

    // Check if there are enough frames for calibration
    if (corner_list.size() >= 5)
    {

        cv::Mat camera_matrix = cv::Mat::eye(3, 3, CV_64F);
        camera_matrix.at<double>(0, 0) = frame.cols;       // initial guess for fx
        camera_matrix.at<double>(1, 1) = frame.rows;       // initial guess for fy
        camera_matrix.at<double>(0, 2) = frame.cols / 2.0; // cx
        camera_matrix.at<double>(1, 2) = frame.rows / 2.0; // cy

        std::vector<double> distortion_coefficients;

        // Print the camera matrix and the distortion coefficients before calibration
        std::cout << "Camera matrix before calibration: " << camera_matrix << std::endl;
        std::cout << "Distortion coefficients before calibration: " << std::endl;
        for (const auto &coeff : distortion_coefficients)
        {
            std::cout << coeff << " ";
        }
        std::cout << std::endl;

        // Run the calibration
        std::vector<cv::Mat> rotations, translations;
        double error = cv::calibrateCamera(point_list, corner_list, frame.size(), camera_matrix, distortion_coefficients, rotations, translations, cv::CALIB_FIX_ASPECT_RATIO);

        // Print the camera matrix and the distortion coefficients after calibration
        std::cout << "Camera matrix after calibration: " << camera_matrix << std::endl;
        std::cout << "Distortion coefficients after calibration: " << std::endl;
        for (const auto &coeff : distortion_coefficients)
        {
            std::cout << coeff << " ";
        }
        std::cout << std::endl;

        std::cout << "Final re-projection error: " << error << std::endl;

        // Save the transformation matrices
        for (int i = 0; i < rotations.size(); ++i)
        {
            std::string filename = "E:/MSCS/CVPR/projects/project4/task1/data/calibration/calibration_params/rotation_" + std::to_string(i) + ".xml";
            cv::FileStorage fs(filename, cv::FileStorage::WRITE);
            fs << "Rotation" << rotations[i];
            fs.release();

            filename = "E:/MSCS/CVPR/projects/project4/task1/data/calibration/calibration_params/translation_" + std::to_string(i) + ".xml";
            fs.open(filename, cv::FileStorage::WRITE);
            fs << "Translation" << translations[i];
            fs.release();
        }

        // Save the camera matrix and the distortion coefficients
        cv::FileStorage fs("E:/MSCS/CVPR/projects/project4/task1/data/calibration/calibration_params/intrinsic_parameters.xml", cv::FileStorage::WRITE);
        fs << "CameraMatrix" << camera_matrix;
        fs << "DistortionCoefficients" << cv::Mat(distortion_coefficients);
        fs.release();
    }
    else
    {
        std::cout << "Please select upto 5 frames for calibration\n"; // If there are not enough frames
    }
}