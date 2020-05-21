#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "processingHelpers.h";
#include "thresholdHelpers.h";

const double PI = 3.14159265358979323846;

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

Mat findInnerCircle(Mat* binaryImage, Mat* palmPointImage, const std::pair<double, double> &palmPoint) {

  std::cout << "Center X: " << palmPoint.first << ", Center Y: " << palmPoint.second << std::endl;
  bool flag = true;
  int radius = 1;
  //Mat palmMaskImage(binaryImage->size().height, binaryImage->size().width, 1, Scalar(0));
  Mat palmMaskImage = createBinaryThresholdImage(*palmPointImage);

  while (flag) {
    
    for (int i = 0; i < 360; i += 1) {
      double x = radius * cos((i* PI)/180) + palmPoint.first;
      double y = radius * sin((i* PI)/ 180) + palmPoint.second;
      
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
    circle(*palmPointImage, Point(palmPoint.first, palmPoint.second), radius*1.5, Scalar(255, 0, 0), 1, 8, 0);

    /*radius *= 1.5;    
      
    for (int p = 0; p < binaryImage->size().height; ++p) {
      for (int q = 0; q < binaryImage->size().width; ++q) {
        
        double distance = sqrt(pow(p - palmPoint.first, 2) + pow(q - palmPoint.second, 2));
        int binaryVal = binaryImage->at<uchar>(p, q);
        if (distance < radius) {
          palmMaskImage.at<uchar>(p, q) = binaryVal;
        }
        else {
          palmMaskImage.at<uchar>(p, q) = 0;
        }
      }
    }*/
  }
  return palmMaskImage;
}


void findAllContours(Mat* binaryImage, std::vector<std::vector<Point>> &contours, int& maxContourIndex){

  int maxValue = -1;
  std::vector<Vec4i> heirarchy;
  findContours(*binaryImage, contours, heirarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
  std::cout << "size: " << contours.size() << std::endl;
  
  for (int i = 0; i < contours.size(); ++i) {
    if(contours[i].size() > maxValue){
      maxValue = contours[i].size();
      maxContourIndex = i;
    }
  }
}