//
//  CameraCalibration.hpp
//  ARTest
//
//  Created by 郭子乐 on 2017/12/13.
//  Copyright © 2017年 郭子乐. All rights reserved.
//

#ifndef CameraCalibration_hpp
#define CameraCalibration_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>

using namespace std;
using namespace cv;

class CameraCalibration
{
public:
    Ptr<aruco::Dictionary> dictionary;
    Ptr<aruco::CharucoBoard> charuco_board;
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;
    
    CameraCalibration();
    void printCharucoBoard(string root_path="Calibration");
    void calibrateCamera(string root_path="Calibration/photos", int image_num=11);
    void takePhotos(string root_path="Calibration/photos");
};


#endif /* CameraCalibration_hpp */
