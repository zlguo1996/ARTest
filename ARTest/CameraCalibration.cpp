//
//  CameraCalibration.cpp
//  ARTest
//
//  Created by 郭子乐 on 2017/12/13.
//  Copyright © 2017年 郭子乐. All rights reserved.
//

#include "CameraCalibration.hpp"

CameraCalibration::CameraCalibration(){
    dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_50);
    charuco_board = aruco::CharucoBoard::create(5, 7, 0.04, 0.02, dictionary);
}

void CameraCalibration::printCharucoBoard(string root_path){
    Mat charuco_board_image;
    charuco_board->draw(cv::Size(600, 500), charuco_board_image, 10, 1);
    imwrite(root_path+"/CharucoBoard.jpg", charuco_board_image);
}

void CameraCalibration::calibrateCamera(string root_path, int image_num){
    // read images
    vector<Mat> calibrateImages;
    for (int i=0; i<image_num; i++) {
        string path = root_path+"/"+to_string(i)+".jpg";
        calibrateImages.push_back(imread(path));
    }
    
    cv::Size imgSize = Size(calibrateImages[0].rows,calibrateImages[0].cols); // camera image size
    std::vector<std::vector<cv::Point2f>> allCharucoCorners;
    std::vector<std::vector<int>> allCharucoIds;
    
    // Detect charuco board from several viewpoints and fill allCharucoCorners and allCharucoIds
    for (int i=0; i<calibrateImages.size(); i++) {
        vector<vector<Point2f>> markerCorners;
        vector<int> markerIds;
        aruco::detectMarkers(calibrateImages[i], dictionary, markerCorners, markerIds);
        if (markerIds.size()==0) {
            throw "no markers";
        }
        vector<Point2f> charucoCorners;
        vector<int> charucoIds;
        aruco::interpolateCornersCharuco(markerCorners, markerIds, calibrateImages[i], charuco_board, charucoCorners, charucoIds);
        allCharucoCorners.push_back(charucoCorners);
        allCharucoIds.push_back(charucoIds);
    }

    // After capturing in several viewpoints, start calibration
    std::vector<cv::Mat> rvecs, tvecs;
    int calibrationFlags = 0; // Set calibration flags (same than in calibrateCamera() function)
    double repError = cv::aruco::calibrateCameraCharuco(allCharucoCorners, allCharucoIds, charuco_board, imgSize, cameraMatrix, distCoeffs, rvecs, tvecs, calibrationFlags);
    
    cout << "cameraMatrix:" << cameraMatrix << endl;
    cout << "distCoeffs" << distCoeffs << endl;
}

void CameraCalibration::takePhotos(string root_path){
    Mat frame;
    VideoCapture capture(0);
    int photo_count = 0;
    
    while (capture.isOpened()) {
        capture >> frame;
        imshow("monitor", frame);
        
        int key = waitKey(20);
        if (key == 27)  //esc
        {
            break;
        }else if (key == 'c')   //照相
        {
            string path = root_path+"/"+to_string(photo_count)+".jpg";
            std::cout << "print:" << path << endl;
            imwrite(path, frame);
            photo_count++;
        }
    }
}
