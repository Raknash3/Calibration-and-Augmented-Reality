/*
 * File name: operations_task1.h
 * Created on : 17th Feb 2024
 * Authors: Ravi Shankar Sankara Narayanan , NUID: 001568628
 * Purpose: This files contains the declarations of the functions implemented in the operations_task1.cpp file.
 */

#include <opencv2/opencv.hpp>
#include <vector>

// Function headers

void findChessboardCorners(cv::Mat &frame, std::vector<std::vector<cv::Point2f>> &corner_list, std::vector<std::vector<cv::Vec3f>> &point_list, int key);
void runCalibration(std::vector<std::vector<cv::Point2f>> &corner_list, std::vector<std::vector<cv::Vec3f>> &point_list, cv::Mat &frame);