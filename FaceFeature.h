//
// Created by heihei on 2020/5/13.
//

#ifndef QT_OPENCV_FACEFEATURE_H_
#define QT_OPENCV_FACEFEATURE_H_

#include <opencv2/opencv.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>

#include "FaceMakeup.h"


class FaceFeature
{
public:
    explicit FaceFeature(cv::Mat &img);


    ~FaceFeature();


    void findFeature(cv::Mat &img);


    cv::Mat transformFacePoint(cv::Mat &img, double change, const cv::Rect &face);


    cv::Mat transformEyePoint(cv::Mat &img, double change, const cv::Rect &face);


    void changeLandmarks(const dlib::full_object_detection &d, std::vector<cv::Point2f> &landmarks);


    cv::Rect findFace(cv::Mat &img);


    cv::Mat smallFace(double degree);


    cv::Mat bigEye(double degree);


    cv::Mat smoothFace(int value_1, double value_2);


    cv::Mat LightFace(int model, double value_1, double value_2);


    cv::Mat redMouth(int value);


    void setNewImage(cv::Mat &img);

private:
    static void drawPolyline(cv::Mat &img, const dlib::full_object_detection &d, int start, int end, bool isClosed);


    static void renderFace(cv::Mat &img, const dlib::full_object_detection &d);


    static cv::Mat
    transformPoint(std::vector<cv::Point2f> control_p, std::vector<cv::Point2f> control_q, int img_i, int img_j);

private:
    dlib::frontal_face_detector detector;

    dlib::shape_predictor pose_model;

    cv::CascadeClassifier face_cascade;

    cv::Mat face_img;

    dlib::full_object_detection face_feature;

    cv::Rect face_rect;


};



#endif //QT_OPENCV_FACEFEATURE_H_
