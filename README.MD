# Hand Gesture Recognition

A program written using openCV libraries that uses computer vision techniques to detect hands and fingers from an input webcam.

## Computational Vision Class Project
Written by: Parakram Basnet
Instructot: Ioannis Stamos
Class: Computational Vision

## Installation

1. Download the OpenCV library and clone the repo into a folder.
2. Use visual studio to open the sln file and start the build.

## File Structure

In this repo, there its the solution style for import into visual studio, and a directory "./Hand Gesture Recognition" that contains the code for the project.

## Files

* main.cpp: 
	* Driver program for the application. 
	* Contains function calls to the helper functions that handle tasks like creating threshold images and processing.
	* Reads input from webcam and creates Mat objects to store them in.
	* Displays all the final windows and outputs.

* thresholdHelpers.h and thresholdHelpers.cpp.
	* Program and interface for helper functions that run algorithms for image preprocessing.
	* Contains logic for HSV mask, creating binary images and distance transformation map.
	
* processingHelpers.h and processingHelpers.cpp.
	* Program and interface for helper functions that run algorithms related to hand detection.
	* Contains algorithm for displaying contours, counting fingers and draw palm mask.
	