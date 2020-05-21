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
  
  //locate the palm point on the image
  circle(palmPointImage, Point(maxLoc.x, maxLoc.y), 5, Scalar(0,255,0), 2, 8, 0);

  return palmPointImage;
}

void findInnerCircle(Mat* binaryImage, Mat* palmPointImage, const std::pair<double, double> &palmPoint) {

  //std::cout << "Center X: " << palmPoint.first << ", Center Y: " << palmPoint.second << std::endl;
  bool flag = true;
  int radius = 1;
  //Mat palmMaskImage = createBinaryThresholdImage(*palmPointImage);

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

    // palm mask and wrist line code INCOMPLETE
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
}


void findAllContours(Mat* binaryImage, std::vector<std::vector<Point>> &contours, int &maxContourIndex){

  int maxValue = 0;
  std::vector<Vec4i> heirarchy;
  findContours(*binaryImage, contours, heirarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
  
  for (int i = 0; i < contours.size(); ++i) {

    if(contours[i].size() > maxValue){
      maxValue = contours[i].size();
      maxContourIndex = i;
    }
  }
}

int findHullsAndDefects( Mat *image,
                          std::vector<std::vector<Point>>& contours,
                          const int& contourSize,
                          std::vector<std::vector<int>>& hull,
                          std::vector<std::vector<Point>>& hullPixels,
                          std::vector<std::vector<Vec4i>>& defects,
                          std::vector<std::vector<Point>>& defectPixels,
                          const int& maxContourIndex) {

  int fingertips = 0;
  for (int i = 0; i < contourSize; ++i) {

    if (contourArea(contours[i]) <= 5000) {
      continue;
    }

    convexHull(contours[i], hull[i], true);
    convexityDefects(contours[i], hull[i], defects[i]);

    if (maxContourIndex == i) {

      for (int j = 0; j < hull[i].size(); ++j) {
        hullPixels[i].push_back(contours[i][hull[i][j]]);
      }

      for (int j = 0; j < defects[i].size(); ++j) {
        if (defects[i][j][3] > 12 * 255) {

          defectPixels[i].push_back(contours[i][defects[i][j][2]]);
          circle(*image, contours[i][defects[i][j][1]], 3, Scalar(0, 0, 255), 2);
          fingertips++;
        }
      }
    }
  }

  return fingertips;
}


std::string getDisplayMessage(const int& fingers) {

  std::string message;

  switch (fingers){
  case 0:
    message = "Detected no fingers. Place hand in Green box.";
    break;
  case 1:
    message = "Detected 1 finger.";
    break;  
  case 2:
    message = "Detected 2 fingers.";
    break;
  case 3:
    message = "Detected 3 fingers.";
    break;

  case 4:
    message = "Detected 4 fingers.";
    break;

  case 5:
    message = "Detected 5 fingers.";
    break;

  default:
    message = "Place Hand in Green Box to begin.";
    break;
  }

  return message;
}