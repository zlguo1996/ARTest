//
//  PoseEstimation.hpp
//  ARTest
//
//  Created by 郭子乐 on 2017/12/28.
//  Copyright © 2017年 郭子乐. All rights reserved.
//

#ifndef PoseEstimation_hpp
#define PoseEstimation_hpp

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

#include "CameraCalibration.hpp"
#include "Markers.hpp"
#include "Tools.hpp"

using namespace std;
using namespace cv;

class PoseEstimation
{
public:
    PoseEstimation();
    
    bool getCharucoCornersIds(const Mat& image, const CameraCalibration& cc, vector<Point2f>& charucoCorners, vector<int>& charucoIds);
    void constructAxis(const Mat& image, const CameraCalibration& cc);   //以棋盘格的位置建立坐标系
    
    bool charucoBoardPoseEstimation(const Mat& image, const Markers& m, const CameraCalibration& cc, Mat& viewMatrix);
    bool charucoMarkersPoseEstimation(const Mat& image, const Markers& m, const CameraCalibration& cc, vector<Mat>& viewMatrixs, vector<int>& ids);
    
    Mat cameraViewMatrix;
};

#endif /* PoseEstimation_hpp */
