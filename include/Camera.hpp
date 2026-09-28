#pragma once

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <string>

class Camera {
public:
    Camera() = default;
    ~Camera();

    bool open(int cameraIndex = 0);
    bool read(cv::Mat& frame);
    void release();
    bool isOpened() const;

    int getWidth() const;
    int getHeight() const;
    double getFPS() const;

    void setResolution(int width, int height);

private:
    cv::VideoCapture capture_;
    bool opened_ = false;
};