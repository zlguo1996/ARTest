//
//  PoseEstimation.cpp
//  ARTest
//
//  Created by 郭子乐 on 2017/12/28.
//  Copyright © 2017年 郭子乐. All rights reserved.
//

#include "PoseEstimation.hpp"

PoseEstimation::PoseEstimation(){
    
}

bool PoseEstimation::getCharucoCornersIds(const Mat& image, const CameraCalibration& cc, vector<Point2f>& charucoCorners, vector<int>& charucoIds) const {
    vector<vector<Point2f>> corners;
    vector<int> ids;
    aruco::detectMarkers(image, cc.dictionary, corners, ids);
    if (ids.size()==0) return false;
    else {
        aruco::interpolateCornersCharuco(corners, ids, image, cc.charuco_board, charucoCorners, charucoIds);
        return true;
    }
}

void PoseEstimation::constructAxis(const Mat& image, const CameraCalibration& cc){   //以棋盘格中心建立坐标系，得到相机位置
    vector<Point2f> charucoCorners;
    vector<int> charucoIds;
    getCharucoCornersIds(image, cc, charucoCorners, charucoIds);
    Mat rvec, tvec;
    aruco::estimatePoseCharucoBoard(charucoCorners, charucoIds, cc.charuco_board, cc.cameraMatrix, cc.distCoeffs, rvec, tvec);
    getViewMatrixFromRvecTvec(rvec, tvec, cameraViewMatrix);
}

bool PoseEstimation::charucoBoardPoseEstimation(const Mat& image, const Markers& m, const CameraCalibration& cc, Mat& viewMatrix) const{
    vector<Point2f> charucoCorners;
    vector<int> charucoIds;
    bool flag = getCharucoCornersIds(image, cc, charucoCorners, charucoIds);
    if (!flag) return false;
    
    Mat rvec, tvec;
    bool valid = aruco::estimatePoseCharucoBoard(charucoCorners, charucoIds, m.charuco_board, cc.cameraMatrix, cc.distCoeffs, rvec, tvec);
    if (!valid) return false;
    getViewMatrixFromRvecTvec(rvec, tvec, viewMatrix);
    return true;
}
bool PoseEstimation::charucoBoardPoseEstimation(const Mat& image, const Markers& m, const CameraCalibration& cc, Mat& tvec, Mat& rvec) const{
    vector<Point2f> charucoCorners;
    vector<int> charucoIds;
    bool flag = getCharucoCornersIds(image, cc, charucoCorners, charucoIds);
    if (!flag) return false;
    
    bool valid = aruco::estimatePoseCharucoBoard(charucoCorners, charucoIds, m.charuco_board, cc.cameraMatrix, cc.distCoeffs, rvec, tvec);
    if (!valid) return false;
    return true;
}

// have bug （关于marker的size和标定用的board的size不统一）
bool PoseEstimation::singleMarkersPoseEstimation(const Mat& image, const Markers& m, const CameraCalibration& cc, vector<Mat>& viewMatrixs, vector<int>& ids) const{
    viewMatrixs.clear();
    ids.clear();
    
    vector<vector<Point2f>> corners;
    aruco::detectMarkers(image, m.dictionary, corners, ids);
    if (ids.empty()) {
        cout << "WARNING: Can't find marker" << endl;
        return false;
    }
    
    vector<Mat> rvecs, tvecs;
    aruco::estimatePoseSingleMarkers(corners, m.markerLength, cc.cameraMatrix, cc.distCoeffs, rvecs, tvecs);
    
    for (int i=0; i<ids.size(); i++) {
        Mat viewMatrix;
        getViewMatrixFromRvecTvec(rvecs[i], tvecs[i], viewMatrix);
        viewMatrixs.push_back(viewMatrix);
    }
    
    return true;
}
bool PoseEstimation::singleMarkersPoseEstimation(const Mat& image, const Markers& m, const CameraCalibration& cc, vector<Mat>& tvecs, vector<Mat>& rvecs, vector<int>& ids) const{
    tvecs.clear();
    rvecs.clear();
    ids.clear();
    
    vector<vector<Point2f>> corners;
    aruco::detectMarkers(image, m.dictionary, corners, ids);
    if (ids.empty()) {
        cout << "WARNING: Can't find marker" << endl;
        return false;
    }
    
    aruco::estimatePoseSingleMarkers(corners, m.markerLength, cc.cameraMatrix, cc.distCoeffs, rvecs, tvecs);
    
    return true;
}
