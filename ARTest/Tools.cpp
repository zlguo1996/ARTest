//
//  Tools.cpp
//  ARTest
//
//  Created by 郭子乐 on 2017/12/28.
//  Copyright © 2017年 郭子乐. All rights reserved.
//

#include "Tools.hpp"

void getViewMatrixFromRvecTvec(InputArray& rvec, InputArray& tvec, Mat& viewMatrix) {
    Mat rotation;
    Mat rvector = rvec.getMat();
    Mat tvector = tvec.getMat();
    viewMatrix = Mat(4, 4, CV_64F);
    cv::Rodrigues(rvector, rotation);
    
    for(unsigned int row=0; row<3; ++row)
    {
        for(unsigned int col=0; col<3; ++col)
        {
            viewMatrix.at<double>(row, col) = rotation.at<double>(row, col);
        }
        viewMatrix.at<double>(row, 3) = tvector.at<double>(row, 0);
    }
    viewMatrix.at<double>(3, 3) = 1.0f;
}

void getRvecTvecFromViewMatrix(InputArray& viewMatrix, Mat& rvec, Mat& tvec){
    Mat vm = viewMatrix.getMat();
    rvec = Mat(3, 1, CV_64F);
    tvec = Mat(3, 1, CV_64F);
    Mat rotation = vm(Range(0, 3), Range(0, 3));
    cv::Rodrigues(rotation, rvec);
    tvec = vm(Range(0, 3), Range(3, 4));
}

void cvToGl(const Mat& cv, Mat& gl){
    cv::Mat cvToGl = cv::Mat::zeros(4, 4, CV_64F);
    cvToGl.at<double>(0, 0) = 1.0f;
    cvToGl.at<double>(1, 1) = -1.0f; // Invert the y axis
    cvToGl.at<double>(2, 2) = -1.0f; // invert the z axis
    cvToGl.at<double>(3, 3) = 1.0f;
    gl = cvToGl * cv;
}

void glToCv(const Mat& gl, Mat& cv){
    cv::Mat glToCv = cv::Mat::zeros(4, 4, CV_64F);
    glToCv.at<double>(0, 0) = 1.0f;
    glToCv.at<double>(1, 1) = -1.0f; // Invert the y axis
    glToCv.at<double>(2, 2) = -1.0f; // invert the z axis
    glToCv.at<double>(3, 3) = 1.0f;
    cv = glToCv * gl;
}
