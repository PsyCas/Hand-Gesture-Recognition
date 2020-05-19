#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main () {

	VideoCapture cap(0);
	if (!cap.isOpened()) {
		std::cout << "Unable to detect webcam source. Exiting." << std::endl;
		return -1;
	}

	while (true) {
		Mat frame;
		cap >> frame;
		imshow("Webcam frame", frame);

		if (waitKey(30) == 27) {
			break;
		}
	}
	return 0;
}