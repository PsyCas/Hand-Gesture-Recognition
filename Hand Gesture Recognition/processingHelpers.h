#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;

Mat createPalmPoint(const Mat *inputImage, const Mat* imgROI, std::pair<double, double> &palmPoint);
void findInnerCircle(Mat* binaryImage, Mat *palmPointImage, const std::pair<double, double>& palmPoint);
void findAllContours(Mat* binaryImage, std::vector<std::vector<Point>> &contours, int& maxContourIndex);
int findHullsAndDefects( Mat * image,
                          std::vector<std::vector<Point>> &contours, 
                          const int &contourSize, 
                          std::vector<std::vector<int>> &hull, 
                          std::vector<std::vector<Point>> &hullPixels, 
                          std::vector<std::vector<Vec4i>> &defects, 
                          std::vector<std::vector<Point>> &defectPixels,
                          const int &maxContourIndex);

std::string getDisplayMessage(const int& fingers);