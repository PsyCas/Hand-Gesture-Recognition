#include <opencv2/opencv.hpp>
#include <iostream>

// include helper functions
#include "hsvThresholding.h";

using namespace cv;

int main() {

	// create video capture object
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		std::cout << "Unable to detect webcam source. Exiting." << std::endl;
		return -1;
	}

	namedWindow("HSV Threshold Image", WINDOW_AUTOSIZE);
	namedWindow("Region of Interest", WINDOW_AUTOSIZE);
	// namedWindow("Threshold Image", WINDOW_AUTOSIZE);

	Mat webcamFrame;
	Mat maskImage;
	Mat imgROI;

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

		putText(webcamFrame, "Place Hand Here", Point(330, 80), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 0, 255), 1, 8, false);
		rectangle(webcamFrame, Point(330,90), Point(620,380), Scalar(0, 255, 0), 2, 8, 0);


		// call helper function for hsvThresholding
		Mat hsvImage = createHSVThresholdImage(imgROI, maskImage);

		// display all required frames 
		imshow("Webcam frame", webcamFrame);
		imshow("HSV Threshold Image", hsvImage);
		imshow("Region of Interest", imgROI);

		// Break out logic. If user presses esc, end program
		if (waitKey(30) == 27) {
			break;
		}
	}
	return 0;
}