#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;

Mat createPalmPoint(const Mat *inputImage, const Mat* imgROI, std::pair<double, double> &palmPoint);
int findInnerCircle(Mat* binaryImage, Mat *palmPointImage, const std::pair<double, double>& palmPoint);
