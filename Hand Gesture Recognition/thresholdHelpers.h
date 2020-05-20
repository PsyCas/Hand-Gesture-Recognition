#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;

Mat createHSVThresholdImage(Mat webcamFrame, Mat maskImage);
Mat createBinaryThresholdImage(Mat inputImage);
Mat createDistanceTransformImage(Mat binaryImage);