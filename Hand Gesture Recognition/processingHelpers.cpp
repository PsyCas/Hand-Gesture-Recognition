#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "processingHelpers.h";

//precondition: input image is a greyscale distance transformed image 
Mat createPalmPoint(const Mat *inputImage, const Mat *imgROI) {

  Mat palmPointImage = *imgROI;
  double minVal, maxVal;
  Point minLoc, maxLoc;
  minMaxLoc(*inputImage, &minVal, &maxVal, &minLoc, &maxLoc);

  std::cout << minVal << ", " << maxVal<< ", " << maxLoc.x<< ", " << maxLoc.y<<std::endl;
  
  for (int i = maxLoc.x - 5; i < maxLoc.x + 5; i++) {
    for (int j = maxLoc.y - 5; j < maxLoc.y + 5; j++) {

      // edge cases
      if (i > 0 && j > 0 && i < palmPointImage.size().height && j < palmPointImage.size().width) {
        Vec3b& color = palmPointImage.at<Vec3b>(i, j);
        color[0] = 0;
        color[1] = 0;
        color[2] = 255;
        palmPointImage.at<Vec3b>(i, j) = color;
      }
      else {
        palmPointImage = *imgROI;
      }
    }
  }

  return palmPointImage;
}