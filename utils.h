//
// Created by heihei on 2020/5/19.
//

#ifndef QT_OPENCV_UTILS_H_
#define QT_OPENCV_UTILS_H_
#include <opencv2/opencv.hpp>

uchar getMaxrgb(uchar blue, uchar green, uchar red);

uchar getMinrgb(uchar blue, uchar green, uchar red);

uchar getMaxPixRgb(uchar blue, uchar green, uchar red);

uchar getMinPixRgb(uchar blue, uchar green, uchar red);

cv::Vec3b solveOutBounds(cv::Vec3b pix);


#endif //QT_OPENCV_UTILS_H_
