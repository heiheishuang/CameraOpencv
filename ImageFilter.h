//
// Created by heihei on 2020/5/19.
//

#ifndef QT_OPENCV_IMAGEFILTER_H_
#define QT_OPENCV_IMAGEFILTER_H_

#include <opencv2/opencv.hpp>
#include "utils.h"

class ImageFilter
{
public:
    /**
     * @brief The Constructor of ImageFilter
     */
    ImageFilter();

    /**
     * @brief Destructor of ImageFilter
     */
    ~ImageFilter() = default;

    /**
     * @brief The Filter of change saturation
     * @param img The source image
     * @param value The degree of change
     * @return The mat after change
     */
    static cv::Mat FilterSaturation(cv::Mat &img, int value);

    /**
     * @brief The Filter of change Contrast
     * @param img The source image
     * @param value_alpha value_contrast
     * @param value_beta value_brightness
     * @return The mat after change
     */
    static cv::Mat FilterDistinctive(cv::Mat &img, double value_alpha, int value_beta);

    /**
     * @brief The Filter of change Contrast
     * @param img The source image
     * @param value_alpha The degree of source image
     * @param value_beta The degree of blur image
     * @return The mat after change
     */
    static cv::Mat FilterSharpen(cv::Mat &img, double value_alpha, double value_beta);

    /**
     * @brief The Filter of Nostalgia Style
     * @param img The source image
     * @return The mat after change
     *
     *      Nostalgia Style
     *      R = 0.393r + 0.769g + 0.189b
     *      G = 0.349r + 0.686g + 0.168b
     *      B = 0.272r + 0.534g + 0.131b
     */
    static cv::Mat FilterNostalgia(cv::Mat &img);

    /**
     * @brief The Filter of Ground Glass Style
     * @param img The source image
     * @return The mat after change
     */
    static cv::Mat FilterGroundGlass(cv::Mat &img, int value);

    /**
     * @brief The Filter of Comics Style
     * @param img The source image
     * @return The mat after change
     *
     *      Comics Style
     *      R = abs(g - b + g + r) * r / 256
     *      G = abs(b - g + b + r) * r / 256
     *      B = abs(b - g + b + r) * g / 256
     */
    static cv::Mat FilterComics(cv::Mat &img);

    /**
     * @brief The Filter of Comics Style
     * @param img The source image
     * @param value The degree of value
     * @return The mat after change
     */
    static cv::Mat FilterEmergence(cv::Mat &img, double value);

    /**
     * @brief The Filter of Vignetting Style
     * @param img img The source image
     * @param radius The degree of value radius
     * @param power The degree of value power
     * @return The mat after change
     */
    static cv::Mat FilterVignetting(cv::Mat &img, double radius, double power);

private:
    static double getDistance(const cv::Point& point_1, const cv::Point& point_2);

    static double getMaxDisFromCorners(const cv::Size& imgSize, const cv::Point& center);

    static void generateGradient(cv::Mat &mask, double radius, double power);



};




#endif //QT_OPENCV_IMAGEFILTER_H_
