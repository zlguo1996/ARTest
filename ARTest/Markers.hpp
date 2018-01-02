//
//  PrintMarkers.hpp
//  ARTest
//
//  Created by 郭子乐 on 2017/12/13.
//  Copyright © 2017年 郭子乐. All rights reserved.
//

#ifndef PrintMarkers_hpp
#define PrintMarkers_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>

using namespace std;
using namespace cv;

class Markers{
public:
    Markers();
    
    void printMarkers(string root_path="Markers", int marker_num=12);
    
    Ptr<aruco::Dictionary> dictionary;
    Ptr<aruco::CharucoBoard> charuco_board;
    float markerLength;
    float squareLength;
};

#endif /* PrintMarkers_hpp */
