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

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera = Camera(SCR_WIDTH, SCR_HEIGHT);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

// tracking status
bool isTracking = false;

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
    CameraCalibration cc;
    cc.calibrateCamera();
    
    PoseEstimation pe;
    Markers m;
    
    VideoCapture inputVideo;
    inputVideo.open(0);
    
    while (!glfwWindowShouldClose(window)) {
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
