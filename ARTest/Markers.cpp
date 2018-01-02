//
//  PrintMarkers.cpp
//  ARTest
//
//  Created by 郭子乐 on 2017/12/13.
//  Copyright © 2017年 郭子乐. All rights reserved.
//

#include "Markers.hpp"

Markers::Markers(){
    dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_50);
    charuco_board = aruco::CharucoBoard::create(5, 7, 0.04, 0.02, dictionary);
    markerLength = charuco_board->getMarkerLength();
    squareLength = charuco_board->getSquareLength();
}

void Markers::printMarkers(string root_path, int marker_num){
    vector<Mat> markers;
    
    for (int i=0; i<marker_num; i++) {
        Mat markerImage;
        aruco::drawMarker(dictionary, i, 100, markerImage);
        markers.push_back(markerImage);
    }
    
    for (int i=0; i<marker_num; i++) {
        imwrite(root_path+"/"+to_string(i)+".jpg", markers[i]);
    }
}
