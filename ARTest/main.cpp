//
//  main.cpp
//  ARTest
//
//  Created by 郭子乐 on 2017/12/13.
//  Copyright © 2017年 郭子乐. All rights reserved.
//

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

#include "Markers.hpp"
#include "CameraCalibration.hpp"
#include "PoseEstimation.hpp"

using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) {
    CameraCalibration cc;
    cc.calibrateCamera();
    
    PoseEstimation pe;
    Markers m;
    
    VideoCapture inputVideo;
    inputVideo.open(0);
    
    bool isTracking = false;

    while (inputVideo.grab()) {
        Mat image, image_copy;
        inputVideo.retrieve(image);
        image.copyTo(image_copy);

        //绘制marker位置
//        vector<int> ids;
//        vector<vector<Point2f>> corners;
//        aruco::detectMarkers(image, cc.dictionary, corners, ids);
//        if (ids.size()>0) {
//            aruco::drawDetectedMarkers(image_copy, corners, ids);
//        }

        char key = (char) cv::waitKey(50);
        if (key == 27) break;
        switch (key) {
            case 'c':
                pe.constructAxis(image, cc);
                cout << pe.cameraViewMatrix << endl;
                cout << "start tracking" << endl;
                isTracking = true;
                break;
            case 't':
                isTracking = !isTracking;
                break;
        }
        
        if (isTracking) {
            Mat viewMatrix;
            bool flag = pe.charucoBoardPoseEstimation(image, m, cc, viewMatrix);
            if (flag) {  // 恢复位置
                Mat trueviewm = pe.cameraViewMatrix.inv()*viewMatrix;
                string text = "offset: " + to_string(trueviewm.at<double>(0, 3)) + "," + to_string(trueviewm.at<double>(1, 3)) + "," + to_string(trueviewm.at<double>(2, 3));
                putText(image_copy, text, Point(0.0, 50.0), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0,0,255));
                
                //绘制charuco的坐标
                Mat rvec, tvec;
                getRvecTvecFromViewMatrix(viewMatrix, rvec, tvec);
                aruco::drawAxis(image_copy, cc.cameraMatrix, cc.distCoeffs, rvec, tvec, 0.1);
            }else { // 无法恢复位置
                string text = "no offset";
                putText(image_copy, text, Point(0.0, 50.0), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0,0,255));
            }
        }else {
            string text = "no offset";
            putText(image_copy, text, Point(0.0, 50.0), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0,0,255));
        }
        
        imshow("out", image_copy);
    }
    
    return 0;
}



