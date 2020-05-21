//
// Created by heihei on 2020/5/14.
//

#ifndef QT_OPENCV_FACEMAKEUP_H_
#define QT_OPENCV_FACEMAKEUP_H_

#include <opencv2/opencv.hpp>
#include "utils.h"

class FaceMakeup
{
public:
    /**
     * @brief The Constructor of FaceMakeup
     */
    FaceMakeup();

    /**
     * @brief Destructor of FaceMakeup
     */
    ~FaceMakeup();

    /**
     * @brief Smooth all the image
     * @param img The source image
     * @param value_1
     * @param value_2
     * @return The mat after change
     */
    static cv::Mat SmoothImage(cv::Mat &img, int value_1, double value_2);

    /**
     * @brief Be used to smooth the face
     * @param img The source image
     * @param value_1
     * @param value_2
     * @param face_rect The rect of face
     * @return The mat after change
     */
    static cv::Mat SmoothFace(cv::Mat &img, int value_1, double value_2, const cv::Rect &face_rect);

    /**
     * @brief Be used to lightSkin by detect skin color
     * @param img The source image
     * @param value_1 value_contrast
     * @param value_2 value_brightness
     * @return The mat after change
     */
    static cv::Mat LightSkin(cv::Mat &img, double value_1, double value_2);

    /**
     * @brief Be used to lightSkin by detect face feature
     * @param img The source image
     * @param value_1 value_contrast
     * @param value_2 value_brightness
     * @param face_rect The rect of face
     * @return The mat after change
     */
    static cv::Mat LightSkin(cv::Mat &img, double value_1, double value_2, const cv::Rect &face_rect);

    /**
     * @brief LightSkin Be used to lightSkin by detect face point
     * @param img The source image
     * @param value_1 value_contrast
     * @param value_2 value_brightness
     * @param face_point The rect of face
     * @return The mat after change
     */
    static cv::Mat LightSkin(cv::Mat &img, double value_1, double value_2, const std::vector<cv::Point> &face_point);

    /**
     * @brief Be used to red mouth
     * @param img The source image
     * @param value The degree
     * @param mouth_point The point of mouth
     * @return The mat after red mouth
     */
    static cv::Mat RedMouth(cv::Mat &img, int value, const std::vector<cv::Point> &mouth_point);

    /**
     * @brief Be used to color the image
     * @param img The source image
     * @param value The degree
     * @return The mat after color image
     */
    static cv::Mat ColorImage(cv::Mat &img, int value);


private:
    static bool isInRect(int i, int j, const cv::Rect &rect);

    static bool isInLightSkin(cv::Mat &img, int i, int j);

    static cv::Mat addAlphaChannel(cv::Mat &img);

    static cv::Mat deleteAlphaChannel(cv::Mat &img);

    static cv::Mat addAlphaChannel(cv::Mat &img, cv::Mat &alpha);

    static cv::Mat createAlpha(cv::Mat &img);

    static cv::Mat hsvSkinDetector(cv::Mat &src);

    static void getStraightLineAllPoint(int x0, int y0, int x1, int y1, std::vector<cv::Point> &points);

    static cv::Mat getUpperLip(cv::Mat &img, std::vector<cv::Point> mouth_point);

    static cv::Mat getLowerLip(cv::Mat &img, std::vector<cv::Point> mouth_point);

    static cv::Mat getAllFace(cv::Mat &img, std::vector<cv::Point> face_point);

    static cv::Mat getLeftEye(cv::Mat &img, std::vector<cv::Point> eye_point);

    static cv::Mat getRightEye(cv::Mat &img, std::vector<cv::Point> eye_point);

    static cv::Mat colorMouth(cv::Mat &face_img, cv::Mat &img, int value);

    static cv::Mat LightFace(cv::Mat &img, cv::Mat &face_img, double value_1, double value_2);

    static cv::Vec3b adjustSaturationAdjust(const cv::Vec3b &pix, int value);


};



#endif //QT_OPENCV_FACEMAKEUP_H_
