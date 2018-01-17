//
//  openglmain.cpp
//  ARTest
//
//  Created by 郭子乐 on 2018/1/1.
//  Copyright © 2018年 郭子乐. All rights reserved.
//

// general
#include <iostream>
#include <cmath>
#include <vector>

// opencv
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

#include "Markers.hpp"
#include "CameraCalibration.hpp"
#include "PoseEstimation.hpp"

// opengl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Graphics.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace cv;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera = Camera(SCR_WIDTH, SCR_HEIGHT);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

// tracking
bool isTracking = false;
Mat currentFrameImage;
CameraCalibration cc;
PoseEstimation pe;
Markers m;

GLFWwindow* initGlfwGlew();

// input
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void process_key(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void process_key_test(GLFWwindow* window, Points& point){
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        int num = point.getVertexNum();
        point.addPoint(glm::vec3(0.01*num,0.01*num,0.01*num));
        cout << "add point" << endl;
    }
}

int main(){
    // init opengl
    // 初始化窗口
    GLFWwindow* window = initGlfwGlew();
    
    // 绑定回调事件
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    Grid grid = Grid();
    Axis axis = Axis();
    Points points = Points();
    
    // init opencv
    cc.calibrateCamera();
    
    VideoCapture inputVideo;
    inputVideo.open(0);
    
    while (!glfwWindowShouldClose(window)) {
        // OPENGL
        glfwPollEvents();
        process_key(window);
        
        //test
        process_key_test(window, points);
        
        // timing
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // clear
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // draw
        grid.draw(camera.GetViewMatrix(), camera.GetProjectionMatrix());
        axis.draw(camera.GetViewMatrix(), camera.GetProjectionMatrix());
        points.draw(camera.GetViewMatrix(), camera.GetProjectionMatrix());
        
        glfwSwapBuffers(window);
        
        // OPENCV
        if (inputVideo.grab()) {
            Mat image, image_copy;
            inputVideo.retrieve(image);
            image.copyTo(image_copy);
            image.copyTo(currentFrameImage);
            
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
                    
                    glm::vec3 position = glm::vec3((float)trueviewm.at<double>(0, 3)*10, (float)trueviewm.at<double>(2, 3)*10, -(float)trueviewm.at<double>(1, 3)*10);
                    text = "offset: " + to_string(position.x) + "," + to_string(position.y) + "," + to_string(position.z);
                    cout << text << endl;
                    points.addPoint(position);
                }else { // 无法恢复位置
                    string text = "no offset";

                    putText(image_copy, text, Point(0.0, 50.0), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0,0,255));
                }
            }else {
                string text = "no offset";
                putText(image_copy, text, Point(0.0, 50.0), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0,0,255));
            }

            //glDrawPixels(image_copy.cols, image_copy.rows, GL_RGB, GL_UNSIGNED_BYTE, image_copy.data);

            //imshow("out", image_copy);
        }
    }
    
    glfwTerminate();
    
    return 0;
}

GLFWwindow* initGlfwGlew(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glewExperimental = GL_TRUE;
    if (glewInit()!=GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    
    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    camera.ProcessFrameBufferSize(width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (key==GLFW_KEY_ESCAPE && action==GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key==GLFW_KEY_C && action==GLFW_PRESS) {    // 标记相机位置
        if (currentFrameImage.empty()) {
            cout << "ERROR: No current frame" << endl;
            return;
        }
        pe.constructAxis(currentFrameImage, cc);
        cout << pe.cameraViewMatrix << endl;
        cout << "start tracking" << endl;
        isTracking = true;
    }
    if (key==GLFW_KEY_T && action==GLFW_PRESS) {    // 更改跟踪状态
        if (pe.cameraViewMatrix.empty()) {
            cout << "ERROR: Camera haven't been calibrated" << endl;
        }
        isTracking = !isTracking;
    }
}
void process_key(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
