#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "processingHelpers.h";

//precondition: input image is a greyscale distance transformed image 
Mat createPalmPoint(const Mat *inputImage, const Mat *imgROI, std::pair<double, double> &palmPoint) {

  Mat palmPointImage = *imgROI;
  double minVal, maxVal;
  Point minLoc, maxLoc;
  minMaxLoc(*inputImage, &minVal, &maxVal, &minLoc, &maxLoc);

  palmPoint.first = maxLoc.x;
  palmPoint.second = maxLoc.y;
  
  for (int i = maxLoc.x - 3; i < maxLoc.x + 3; i++) {
    for (int j = maxLoc.y - 3; j < maxLoc.y + 3; j++) {

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

int findInnerCircle(Mat* binaryImage, Mat* palmPointImage, const std::pair<double, double> &palmPoint) {

  std::cout << "Center X: " << palmPoint.first << ", Center Y: " << palmPoint.second << std::endl;
  bool flag = true;
  int radius = 1;

  while (flag) {
    
    for (int i = 0; i < 360; i += 1) {
      double x = radius * cos(i) + palmPoint.first;
      double y = radius * sin(i) + palmPoint.second;
      
      if (x > 0 && y > 0 && x < palmPointImage->size().height && y < palmPointImage->size().width) {
        if (binaryImage->at<uchar>(x, y) == 0) {
          flag = false;
          break;
        }
      }
    }
    radius = flag ? radius + 1 : radius;
  }
  
  if (radius != 1) {
    circle(*palmPointImage, Point(palmPoint.first, palmPoint.second), radius, Scalar(0, 0, 255), 1, 8, 0);
  }

  return radius;
}