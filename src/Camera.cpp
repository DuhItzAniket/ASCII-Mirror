#include "Camera.hpp"
#include <iostream>

Camera::~Camera() {
    release();
}

bool Camera::open(int cameraIndex) {
    if (opened_) {
        release();
    }

    capture_.open(cameraIndex, cv::CAP_ANY);
    if (!capture_.isOpened()) {
        std::cerr << "Error: Could not open camera index " << cameraIndex << std::endl;
        return false;
    }

    opened_ = true;
    return true;
}

bool Camera::read(cv::Mat& frame) {
    if (!opened_) {
        return false;
    }
    return capture_.read(frame);
}

void Camera::release() {
    if (opened_) {
        capture_.release();
        opened_ = false;
    }
}

bool Camera::isOpened() const {
    return opened_;
}

int Camera::getWidth() const {
    if (!opened_) return 0;
    return static_cast<int>(capture_.get(cv::CAP_PROP_FRAME_WIDTH));
}

int Camera::getHeight() const {
    if (!opened_) return 0;
    return static_cast<int>(capture_.get(cv::CAP_PROP_FRAME_HEIGHT));
}

double Camera::getFPS() const {
    if (!opened_) return 0.0;
    return capture_.get(cv::CAP_PROP_FPS);
}

void Camera::setResolution(int width, int height) {
    if (opened_) {
        capture_.set(cv::CAP_PROP_FRAME_WIDTH, width);
        capture_.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    }
}