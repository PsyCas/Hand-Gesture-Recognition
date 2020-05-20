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
	// namedWindow("Region of Interest", WINDOW_AUTOSIZE);
	// namedWindow("Threshold Image", WINDOW_AUTOSIZE);

	Mat webcamFrame;
	Mat maskImage;

	while (true) {

		// initialize webcam frame and perform logic 
		if (!cap.read(webcamFrame)) {
			std::cout << "Error: Cannot display webcam image. Exiting.";
			break;
		}

		// put instruction for exiting
		putText(webcamFrame, "Press Esc to exit.", Point(20, webcamFrame.size().height-20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, 8, false);
		
		// call helper function for hsvThresholding
		Mat hsvImage = createHSVThresholdImage(webcamFrame, maskImage);

		// display all required frames 
		imshow("Webcam frame", webcamFrame);
		imshow("HSV Threshold Image", hsvImage);


		// Break out logic. If user presses esc, end program
		if (waitKey(30) == 27) {
			break;
		}
	}
	return 0;
}