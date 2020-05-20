#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;

Mat createPalmPoint(const Mat *inputImage, const Mat* imgROI);