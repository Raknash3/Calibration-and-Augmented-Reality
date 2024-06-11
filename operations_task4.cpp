/*
 * File name: operations_task4.cpp
 * Created on : 18th Feb 2024
 * Authors: Ravi Shankar Sankara Narayanan , NUID: 001568628
 * Purpose: This files contains the function definition for reading the camera parameters and detecting the target and drawing the virtual object on the target.
 */

#include "operations.h"

// Function to read the camera parameters from a file
void readCameraParameters(const std::string &filename, cv::Mat &cameraMatrix, cv::Mat &distCoeffs)
{
    // Read the camera parameters from the file
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    fs["CameraMatrix"] >> cameraMatrix;
    fs["DistortionCoefficients"] >> distCoeffs;
}

// Function to detect the target in the frame
bool detectTarget(cv::Mat &frame, std::vector<cv::Point2f> &corners)
{
    bool found = cv::findChessboardCorners(frame, cv::Size(9, 6), corners);

    if (found && !corners.empty())
    {
        // Draw a circle around the first corner
        cv::circle(frame, corners[0], 10, cv::Scalar(0, 0, 255), 3);
    }

    return found;
}

void getBoardPose(const std::vector<cv::Point2f> &corners, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, cv::Mat &rvec, cv::Mat &tvec)
{
    std::vector<cv::Point3f> objectPoints;

    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            objectPoints.push_back(cv::Point3f(i, j, 0));
        }
    }

    // Use the solvePnP function to get the pose of the target
    cv::solvePnP(objectPoints, corners, cameraMatrix, distCoeffs, rvec, tvec);

    /*
    // Project the 3D origin back onto the 2D image plane
    std::vector<cv::Point2f> projectedPoints;
    cv::projectPoints(std::vector<cv::Point3f>{{0, 0, 0}}, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);

    // Compare the projected origin with the first corner
    std::cout << "Projected origin: " << projectedPoints[0] << std::endl;
    std::cout << "First corner: " << corners[0] << std::endl;
    */
}

// Function to draw the pyramid on the frame
void drawPyramid(cv::Mat &frame, const cv::Mat &rvec, const cv::Mat &tvec, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs)
{
    // Define the 3D points for the virtual object (a pyramid)
    std::vector<cv::Point3f> objectPoints = {{0, 0, 0}, {1, 1, 2}, {1, -1, 2}, {-1, -1, 2}, {-1, 1, 2}}; // Base at origin, four corners at (±1, ±1, 2)

    // Project the object onto the image plane
    std::vector<cv::Point2f> projectedObjectPoints;
    cv::projectPoints(objectPoints, rvec, tvec, cameraMatrix, distCoeffs, projectedObjectPoints);

    // Draw the object on the frame
    cv::line(frame, projectedObjectPoints[0], projectedObjectPoints[1], cv::Scalar(255, 255, 255), 2); // Line from base to corner 1
    cv::line(frame, projectedObjectPoints[0], projectedObjectPoints[2], cv::Scalar(255, 255, 255), 2); // Line from base to corner 2
    cv::line(frame, projectedObjectPoints[0], projectedObjectPoints[3], cv::Scalar(255, 255, 255), 2); // Line from base to corner 3
    cv::line(frame, projectedObjectPoints[0], projectedObjectPoints[4], cv::Scalar(255, 255, 255), 2); // Line from base to corner 4
    cv::line(frame, projectedObjectPoints[1], projectedObjectPoints[2], cv::Scalar(255, 255, 255), 2); // Line from corner 1 to corner 2
    cv::line(frame, projectedObjectPoints[2], projectedObjectPoints[3], cv::Scalar(255, 255, 255), 2); // Line from corner 2 to corner 3
    cv::line(frame, projectedObjectPoints[3], projectedObjectPoints[4], cv::Scalar(255, 255, 255), 2); // Line from corner 3 to corner 4
    cv::line(frame, projectedObjectPoints[4], projectedObjectPoints[1], cv::Scalar(255, 255, 255), 2); // Line from corner 4 to corner 1
}

// Function to draw the cube on the frame
void drawCube(cv::Mat &frame, const cv::Mat &rvec, const cv::Mat &tvec, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs)
{
    // Define the 3D points for the virtual object (a cube)
    std::vector<cv::Point3f> objectPoints = {{0, 0, 2}, {2, 0, 2}, {2, 2, 2}, {0, 2, 2}, // Base points
                                             {0, 0, 5},
                                             {2, 0, 5},
                                             {2, 2, 5},
                                             {0, 2, 5}}; // Top points

    // Project the object onto the image plane
    std::vector<cv::Point2f> projectedObjectPoints;
    cv::projectPoints(objectPoints, rvec, tvec, cameraMatrix, distCoeffs, projectedObjectPoints);

    // Draw the object on the frame
    // Draw base
    cv::line(frame, projectedObjectPoints[0], projectedObjectPoints[1], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[1], projectedObjectPoints[2], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[2], projectedObjectPoints[3], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[3], projectedObjectPoints[0], cv::Scalar(255, 255, 255), 2);

    // Draw top
    cv::line(frame, projectedObjectPoints[4], projectedObjectPoints[5], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[5], projectedObjectPoints[6], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[6], projectedObjectPoints[7], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[7], projectedObjectPoints[4], cv::Scalar(255, 255, 255), 2);

    // Draw sides
    cv::line(frame, projectedObjectPoints[0], projectedObjectPoints[4], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[1], projectedObjectPoints[5], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[2], projectedObjectPoints[6], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[3], projectedObjectPoints[7], cv::Scalar(255, 255, 255), 2);
}

// Function to draw the house on the frame

void drawHouse(cv::Mat &frame, const cv::Mat &rvec, const cv::Mat &tvec, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs)
{
    // Define the 3D points for the virtual object (a house)
    std::vector<cv::Point3f> objectPoints = {{2, 4, 2}, {4, 4, 2}, {4, 6, 2}, {2, 6, 2}, // Base points
                                             {2, 4, 4},
                                             {4, 4, 4},
                                             {4, 6, 4},
                                             {2, 6, 4},  // Top points of the cube
                                             {3, 5, 5}}; // Top point of the roof

    // Project the object onto the image plane
    std::vector<cv::Point2f> projectedObjectPoints;
    cv::projectPoints(objectPoints, rvec, tvec, cameraMatrix, distCoeffs, projectedObjectPoints);

    // Draw the object on the frame
    // Draw base
    cv::line(frame, projectedObjectPoints[0], projectedObjectPoints[1], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[1], projectedObjectPoints[2], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[2], projectedObjectPoints[3], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[3], projectedObjectPoints[0], cv::Scalar(255, 255, 255), 2);

    // Draw top of the cube
    cv::line(frame, projectedObjectPoints[4], projectedObjectPoints[5], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[5], projectedObjectPoints[6], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[6], projectedObjectPoints[7], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[7], projectedObjectPoints[4], cv::Scalar(255, 255, 255), 2);

    // Draw sides of the cube
    cv::line(frame, projectedObjectPoints[0], projectedObjectPoints[4], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[1], projectedObjectPoints[5], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[2], projectedObjectPoints[6], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[3], projectedObjectPoints[7], cv::Scalar(255, 255, 255), 2);

    // Draw roof
    cv::line(frame, projectedObjectPoints[4], projectedObjectPoints[8], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[5], projectedObjectPoints[8], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[6], projectedObjectPoints[8], cv::Scalar(255, 255, 255), 2);
    cv::line(frame, projectedObjectPoints[7], projectedObjectPoints[8], cv::Scalar(255, 255, 255), 2);
}

// Function to draw the diamond on the frame
void drawDiamond(cv::Mat &frame, const cv::Mat &rvec, const cv::Mat &tvec, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs)
{
    // Define the 3D points for the diamond
    std::vector<cv::Point3f> diamondPoints = {{4, 2, 0}, {5, 2, 1}, {4, 2, 2}, {3, 2, 1}, // Bottom part
                                              {4, 2, 3}};                                 // Top part

    // Project the diamond onto the image plane
    std::vector<cv::Point2f> projectedDiamondPoints;
    cv::projectPoints(diamondPoints, rvec, tvec, cameraMatrix, distCoeffs, projectedDiamondPoints);

    // Draw the diamond on the frame
    for (int i = 0; i < 4; i++)
    {
        cv::line(frame, projectedDiamondPoints[i], projectedDiamondPoints[(i + 1) % 4], cv::Scalar(255, 0, 0), 2);
        cv::line(frame, projectedDiamondPoints[i], projectedDiamondPoints[4], cv::Scalar(255, 0, 0), 2);
    }
}