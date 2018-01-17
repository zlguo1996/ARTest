//
//  Dodecahedron.hpp
//  ARTest
//
//  Created by 郭子乐 on 2018/1/4.
//  Copyright © 2018年 郭子乐. All rights reserved.
//

#ifndef Dodecahedron_hpp
#define Dodecahedron_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>

#include "Markers.hpp"
#include "CameraCalibration.hpp"
#include "PoseEstimation.hpp"

using namespace std;
using namespace cv;

typedef double DOF_6[6];

struct MarkerStatus{
    int faceID;                         // 面编号
    Ptr<aruco::Dictionary> dictionary;  // aruco marker所属的字典
    int markerID;                       // aruco marker的编号
    DOF_6 dof;                          // marker相对于正十二面体的位置和朝向
};

struct FrameStatus{
    int frameCount;     // 第几帧
    float time;         // 该帧的时间
    bool isInterFrame;  // 是直接通过marker位置求出的还是通过光流求出的
    DOF_6 dof;          // 位置和朝向(tvec&rvec)
};

class Dodecahedron{
public:
    vector<MarkerStatus> markerStatusList;  //正12面体上marker
    vector<FrameStatus> frameStatusList;
    
    Dodecahedron(){
        
    }
    
    void calibrateDodecahedron(string rootPath, int imageNum){
        vector<Mat> dodecaImages;
        for (int i=0; i<imageNum; i++) {
            string path = rootPath+"/"+to_string(i)+".jpg";
            Mat image = imread(path);
            if(image.empty()) cout<<"ERROR: Can't read dodecahedron image"<<endl;
            dodecaImages.push_back(image);
        }
    }
    
    void poseEstimation(const Mat& image, const Markers& m, const CameraCalibration& cc, const PoseEstimation& pe){
        vector<Mat> tvecs, rvecs;
        vector<int> ids;
        pe.singleMarkersPoseEstimation(image, m, cc, tvecs, rvecs, ids);
        
        if (ids.size()<2) {
            approximatePoseEstimation();
        }else{
            interframeCornerTracking();
        }
    }
    
private:
    bool approximatePoseEstimation(){
        
        return true;
    }
    bool interframeCornerTracking(){
        
        return true;
    }
    bool densePoseRefinement(){
        
        return true;
    }
    
    DOF_6 predictPose(){
        
    }
};

#endif /* Dodecahedron_hpp */
