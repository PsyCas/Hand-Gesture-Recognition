#include <opencv2/opencv.hpp>
#include <iostream>

// include helper functions
#include "thresholdHelpers.h";
#include "processingHelpers.h";

using namespace cv;

int main() {

	// create video capture object
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		std::cout << "Unable to detect webcam source. Exiting." << std::endl;
		return -1;
	}

	namedWindow("HSV Threshold Image", WINDOW_AUTOSIZE);
	namedWindow("Binary Threshold Image", WINDOW_AUTOSIZE);
	namedWindow("Distance Transform Image", WINDOW_AUTOSIZE);

	Mat webcamFrame;
	Mat maskImage;
	Mat imgROI;
	std::pair<double, double> palmPoint;
	std::vector<std::vector<Point>> contours;
	int maxContourIndex = -1;
	int radius = 0;

	while (true) {

		// initialize webcam frame and perform logic 
		if (!cap.read(webcamFrame)) {
			std::cout << "Error: Cannot display webcam image. Exiting.";
			break;
		}

		// put instruction for exiting
		putText(webcamFrame, "Press Esc to exit.", Point(20, webcamFrame.size().height-20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, 8, false);
		
		//create region of interest
		Rect roi(340, 100, 270, 270);
		imgROI = webcamFrame(roi);

		// locate region of interest in the webcam feed 
		putText(webcamFrame, "Place Hand Here", Point(330, 80), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 0, 255), 1, 8, false);
		rectangle(webcamFrame, Point(330,90), Point(620,380), Scalar(0, 255, 0), 2, 8, 0);

		// call helper function for hsvThresholding
		Mat hsvImage = createHSVThresholdImage(imgROI, maskImage);

		// convert HSV image to binary
		Mat binaryImage = createBinaryThresholdImage(hsvImage);

		// Convert binary image to distance transform image
		Mat distanceTransformImage = createDistanceTransformImage(binaryImage);
		
		// Get Palm point image
		Mat palmPointImage = createPalmPoint(&distanceTransformImage, &imgROI, palmPoint);

		// Draw inner cricle of maximum radius
		findInnerCircle(&binaryImage, &palmPointImage ,palmPoint);

		//Find and get all the contours
		findAllContours(&binaryImage, contours, maxContourIndex);
		int contourSize = contours.size();

		//Find fingertips using convex hulls
		std::vector<std::vector<int>> hull(contourSize);
		std::vector<std::vector<Point>> hullPixels(contourSize);

		std::vector<std::vector<Vec4i>> defects(contourSize);
		std::vector<std::vector<Point>> defectPixels(contourSize);

		int fingersDetected = findHullsAndDefects(&imgROI, contours, contourSize, hull, hullPixels, defects, defectPixels, maxContourIndex);
		
		// show the final message
		std::string displayMessage = "";
		displayMessage = getDisplayMessage(fingersDetected);

		putText(webcamFrame, displayMessage, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 1.5, 8, false);
		
		// display all required frames 
		imshow("Webcam frame", webcamFrame);		// also creates the palm point frame
		imshow("HSV Threshold Image", hsvImage);
		imshow("Binary Threshold Image", binaryImage);
		imshow("Distance Transform Image", distanceTransformImage);
		imshow("Palm Point Image", palmPointImage);
		//imshow("Palm Mask Image", palmMaskImage);

		
		// Break out logic. If user presses esc, end program
		if (waitKey(30) == 27) {
			break;
		}
	}
	return 0;
}