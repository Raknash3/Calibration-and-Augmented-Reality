/*
 * File name: operations_task4.h
 * Created on : 17th Feb 2024
 * Authors: Ravi Shankar Sankara Narayanan , NUID: 001568628
 * Purpose: This files contains the function headers.
 */

#include <opencv2/opencv.hpp>

// Function headers

void readCameraParameters(const std::string &filename, cv::Mat &cameraMatrix, cv::Mat &distCoeffs);
bool detectTarget(cv::Mat &frame, std::vector<cv::Point2f> &corners);
void getBoardPose(const std::vector<cv::Point2f> &corners, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, cv::Mat &rvec, cv::Mat &tvec);
void drawPyramid(cv::Mat &frame, const cv::Mat &rvec, const cv::Mat &tvec, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs);
void drawCube(cv::Mat &frame, const cv::Mat &rvec, const cv::Mat &tvec, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs);
void drawHouse(cv::Mat &frame, const cv::Mat &rvec, const cv::Mat &tvec, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs);
void drawDiamond(cv::Mat &frame, const cv::Mat &rvec, const cv::Mat &tvec, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs);