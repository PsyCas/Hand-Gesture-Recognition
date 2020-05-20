#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "hsvThresholding.h";

using namespace cv;

Mat createHSVThresholdImage(Mat webcamFrame, Mat maskImage) {

	Mat hsvReturn;
	cvtColor(webcamFrame, maskImage, COLOR_BGR2HSV);

	// outlier, red color
	std::vector<int> upperBound = { 30, 255, 255 };
	std::vector<int> lowerBound = { 0, 58, 50 };

	//// add hsv mask to threshold image
	inRange(maskImage, lowerBound, upperBound, maskImage);
	bitwise_and(webcamFrame, webcamFrame, hsvReturn, maskImage);
  return hsvReturn;
}