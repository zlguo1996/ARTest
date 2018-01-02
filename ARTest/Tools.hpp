//
//  Tools.hpp
//  ARTest
//
//  Created by 郭子乐 on 2017/12/28.
//  Copyright © 2017年 郭子乐. All rights reserved.
//

#ifndef Tools_hpp
#define Tools_hpp


#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

using namespace std;
using namespace cv;

void getViewMatrixFromRvecTvec(InputArray& rvec, InputArray& tvec, Mat& viewMatrix);
void getRvecTvecFromViewMatrix(InputArray& viewMatrix, Mat& rvec, Mat& tvec);

//cv - gl : invert axis y,z
void cvToGl(const Mat& cv, Mat& gl);
void glToCv(const Mat& gl, Mat& cv);

#endif /* Tools_hpp */
