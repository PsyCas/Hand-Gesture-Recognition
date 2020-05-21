#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;

Mat createPalmPoint(const Mat *inputImage, const Mat* imgROI, std::pair<double, double> &palmPoint);
Mat findInnerCircle(Mat* binaryImage, Mat *palmPointImage, const std::pair<double, double>& palmPoint);
void findAllContours(Mat* binaryImage, std::vector<std::vector<Point>> &contours, int& maxContourIndex);