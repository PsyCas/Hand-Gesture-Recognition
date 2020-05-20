#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "thresholdHelpers.h";

// convert webcam roi to hsv image
Mat createHSVThresholdImage(Mat webcamFrame, Mat maskImage) {

	Mat hsvReturn;
	cvtColor(webcamFrame, maskImage, COLOR_BGR2HSV);

	// outlier, red color
	std::vector<int> upperBound = { 30, 255, 255 };
	std::vector<int> lowerBound = { 0, 58, 50 };

	// add hsv mask to threshold image
	inRange(maskImage, lowerBound, upperBound, maskImage);
	bitwise_and(webcamFrame, webcamFrame, hsvReturn, maskImage);

  return hsvReturn;
}


// convert hsvThreshold image to greyscale, then binary
Mat createBinaryThresholdImage(Mat inputImage) {
	
	Mat binaryImage;
	
	// convert to greyscale 
	cvtColor(inputImage, binaryImage, COLOR_RGB2GRAY);

	// threshold the value of the greyscale image
	threshold(binaryImage, binaryImage, 0, 255, THRESH_BINARY + THRESH_OTSU);
	
	return binaryImage;
}


Mat createDistanceTransformImage(Mat binaryImage) {
	Mat distanceTransformImage;

	distanceTransform(binaryImage, distanceTransformImage, DIST_L2, 5);
	normalize(distanceTransformImage, distanceTransformImage, 0, 1.0, NORM_MINMAX);
	
	return distanceTransformImage;
}