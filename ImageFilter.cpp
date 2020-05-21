//
// Created by heihei on 2020/5/19.
//

#include "ImageFilter.h"

ImageFilter::ImageFilter()
= default;


cv::Mat ImageFilter::FilterSaturation(cv::Mat &img, int value)
{

    cv::Mat src = img.clone();
    double dPercent = static_cast<double>(value) / 100;

    uchar rgbMax;
    uchar rgbMin;

    double dDelta;
    double dValue;
    double dL;
    double dS;
    double dAlpha;

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            rgbMax = getMaxrgb(src.at<cv::Vec3b>(i, j)[0], src.at<cv::Vec3b>(i, j)[1], src.at<cv::Vec3b>(i, j)[2]);
            rgbMin = getMinrgb(src.at<cv::Vec3b>(i, j)[0], src.at<cv::Vec3b>(i, j)[1], src.at<cv::Vec3b>(i, j)[2]);

            dDelta = static_cast<double>(rgbMax - rgbMin) / 255;
            dValue = static_cast<double>(rgbMax + rgbMin) / 255;
            if (dDelta == 0)
            {
                continue;
            }

            dL = dValue / 2;

            if (dL < 0.5)
            {
                dS = dDelta / dValue;
            }
            else
            {
                dS = dDelta / (2 - dValue);
            }

            if (dPercent >= 0)
            {
                if (dPercent + dS >= 1)
                {
                    dAlpha = dS;
                }
                else
                {
                    dAlpha = 1 - dPercent;
                }

                dAlpha = 1 / dAlpha - 1;

                src.at<cv::Vec3b>(i, j)[0] =
                        src.at<cv::Vec3b>(i, j)[0] + (src.at<cv::Vec3b>(i, j)[0] - dL * 255) * dAlpha;
                src.at<cv::Vec3b>(i, j)[1] =
                        src.at<cv::Vec3b>(i, j)[1] + (src.at<cv::Vec3b>(i, j)[1] - dL * 255) * dAlpha;
                src.at<cv::Vec3b>(i, j)[2] =
                        src.at<cv::Vec3b>(i, j)[2] + (src.at<cv::Vec3b>(i, j)[2] - dL * 255) * dAlpha;
            }
            else
            {
                dAlpha = dPercent;
                src.at<cv::Vec3b>(i, j)[0] = dL * 255 + (src.at<cv::Vec3b>(i, j)[0] - dL * 255) * (1 + dAlpha);
                src.at<cv::Vec3b>(i, j)[1] = dL * 255 + (src.at<cv::Vec3b>(i, j)[1] - dL * 255) * (1 + dAlpha);
                src.at<cv::Vec3b>(i, j)[2] = dL * 255 + (src.at<cv::Vec3b>(i, j)[2] - dL * 255) * (1 + dAlpha);
            }
        }
    }

//    imshow("Filter Saturation", src);
    return src;

}


cv::Mat ImageFilter::FilterDistinctive(cv::Mat &img, double value_alpha, int value_beta)
{
    cv::Mat src = img.clone();

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                uchar tmp = cv::saturate_cast<uchar>(src.at<cv::Vec3b>(i, j)[k] * value_alpha + value_beta);

                if (tmp > 255)
                {
                    src.at<cv::Vec3b>(i, j)[k] = 2 * 255 - tmp;
                }
                else
                {
                    src.at<cv::Vec3b>(i, j)[k] = tmp;
                }
            }
        }
    }

//    cv::imshow("Filter Light", src);
    return src;
}


cv::Mat ImageFilter::FilterSharpen(cv::Mat &img, double value_alpha, double value_beta)
{
    cv::Mat src = img.clone();

    cv::Mat blur_img;
    cv::Mat usm;

    cv::GaussianBlur(src, blur_img, cv::Size(0, 0), 25);
    cv::addWeighted(src, value_alpha, blur_img, value_beta, 0, usm);

//    cv::imshow("Filter Sharpen", usm);

    return usm;
}


cv::Mat ImageFilter::FilterNostalgia(cv::Mat &img)
{
    cv::Mat src = img.clone();

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            cv::Vec3b pix;

            uchar pix_b = img.at<cv::Vec3b>(i, j)[0];
            uchar pix_g = img.at<cv::Vec3b>(i, j)[1];
            uchar pix_r = img.at<cv::Vec3b>(i, j)[2];
            // pix[0] = 0.272 * pix_r + 0.534 * pix_g + 0.131 * pix_b;
            // pix[1] = 0.349 * pix_r + 0.686 * pix_g + 0.168 * pix_b;
            // pix[2] = 0.393 * pix_r + 0.769 * pix_g + 0.189 * pix_b;

            // src.at<cv::Vec3b>(i, j)[0] = pix[0];
            // src.at<cv::Vec3b>(i, j)[1] = pix[1];
            // src.at<cv::Vec3b>(i, j)[2] = pix[2];

            src.at<cv::Vec3b>(i, j)[1] = cv::saturate_cast<uchar>(0.349 * pix_r + 0.686 * pix_g + 0.168 * pix_b);
            src.at<cv::Vec3b>(i, j)[2] = cv::saturate_cast<uchar>(0.393 * pix_r + 0.769 * pix_g + 0.189 * pix_b);
            src.at<cv::Vec3b>(i, j)[0] = cv::saturate_cast<uchar>(0.272 * pix_r + 0.534 * pix_g + 0.131 * pix_b);

        }
    }

//    cv::imshow("Filter Nostalgia", src);

    return src;

}


cv::Mat ImageFilter::FilterGroundGlass(cv::Mat &img, int value)
{
    cv::Mat src = img.clone();
    cv::RNG rng;

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            cv::Vec3b pix;

            int randomNum = rng.uniform(0, value);
            src.at<cv::Vec3b>(i, j)[0] = src.at<cv::Vec3b>(i + randomNum, j + randomNum)[0];
            src.at<cv::Vec3b>(i, j)[1] = src.at<cv::Vec3b>(i + randomNum, j + randomNum)[1];
            src.at<cv::Vec3b>(i, j)[2] = src.at<cv::Vec3b>(i + randomNum, j + randomNum)[2];
        }

    }

//    cv::imshow("Filter GroundGlass", src);

    return src;
}


cv::Mat ImageFilter::FilterComics(cv::Mat &img)
{
    cv::Mat src = img.clone();

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            cv::Vec3b pix;

            uchar pix_b = img.at<cv::Vec3b>(i, j)[0];
            uchar pix_g = img.at<cv::Vec3b>(i, j)[1];
            uchar pix_r = img.at<cv::Vec3b>(i, j)[2];

            uchar R = cv::saturate_cast<uchar>(abs(pix_g - pix_b + pix_g + pix_r) * pix_r / 256);
            uchar G = cv::saturate_cast<uchar>(abs(pix_b - pix_g + pix_b + pix_r) * pix_r / 256);
            uchar B = cv::saturate_cast<uchar>(abs(pix_b - pix_g + pix_b + pix_r) * pix_g / 256);

            uchar gray = (R + G + B) / 3;
            R = 255 < gray + 10 ? 255 : (gray + 10);

            // src.at<cv::Vec3b>(i, j)[0] = cv::saturate_cast<uchar>(B);
            // src.at<cv::Vec3b>(i, j)[1] = cv::saturate_cast<uchar>(G);
            // src.at<cv::Vec3b>(i, j)[2] = cv::saturate_cast<uchar>(R);
            src.at<cv::Vec3b>(i, j)[0] = gray;
            src.at<cv::Vec3b>(i, j)[1] = R;
            src.at<cv::Vec3b>(i, j)[2] = R;

        }
    }

//    cv::imshow("Filter Comics", src);

    return src;

}


cv::Mat ImageFilter::FilterEmergence(cv::Mat &img, double value)
{
    cv::Mat src = img.clone();
    int center_x = src.cols / 2;
    int center_y = src.rows / 2;
    int s2 = center_x * center_x + center_y * center_y;

    double ratio =
            src.cols > src.rows ? static_cast<double>(src.rows) / src.cols : static_cast<double>(src.cols) / src.rows;

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            double dx = static_cast<double>(abs(center_x - j));
            double dy = static_cast<double>(abs(center_y - i));

            if (center_x > center_y)
            {
                dx = dx * ratio;
            }
            else
            {
                dy = dx * ratio;
            }

            double s1 = dx * dx + dy * dy;

            double v = 255 * s1 / (s2 * value);

            int pix_b = src.at<cv::Vec3b>(i, j)[0];
            int pix_g = src.at<cv::Vec3b>(i, j)[1];
            int pix_r = src.at<cv::Vec3b>(i, j)[2];

            src.at<cv::Vec3b>(i, j)[0] = cv::saturate_cast<uchar>(pix_b + v);
            src.at<cv::Vec3b>(i, j)[1] = cv::saturate_cast<uchar>(pix_g + v);
            src.at<cv::Vec3b>(i, j)[2] = cv::saturate_cast<uchar>(pix_r + v);
        }
    }

//    cv::imshow("Filter Emergence", src);
    return src;
}


double ImageFilter::getDistance(const cv::Point &point_1, const cv::Point &point_2)
{
    return sqrt(pow((double) (point_1.x - point_2.x), 2) + pow((double) (point_1.y - point_2.y), 2));
}


double ImageFilter::getMaxDisFromCorners(const cv::Size &imgSize, const cv::Point &center)
{
    // given a rect and a line
    // get which corner of rect is farthest from the line

    std::vector<cv::Point> corners(4);
    corners[0] = cv::Point(0, 0);
    corners[1] = cv::Point(imgSize.width, 0);
    corners[2] = cv::Point(0, imgSize.height);
    corners[3] = cv::Point(imgSize.width, imgSize.height);

    double max_dis = 0;
    for (int i = 0; i < 4; ++i)
    {
        double dis = getDistance(corners[i], center);
        if (max_dis < dis)
        {
            max_dis = dis;
        }
    }

    return max_dis;
}


void ImageFilter::generateGradient(cv::Mat &mask, double radius, double power)
{
    cv::Point first_point = cv::Point(mask.size().width / 2, mask.size().height / 2);

    // max image radian
    double max_image_rad = radius * getMaxDisFromCorners(mask.size(), first_point);

    mask.setTo(cv::Scalar(1));
    for (int i = 0; i < mask.rows; i++)
    {
        for (int j = 0; j < mask.cols; j++)
        {
            double temp = getDistance(first_point, cv::Point(j, i)) / max_image_rad;
            temp = temp * power;
            double temp_s = pow(cos(temp), 4);
            mask.at<double>(i, j) = temp_s;
        }
    }
}


cv::Mat ImageFilter::FilterVignetting(cv::Mat &img, double radius, double power)
{
    cv::Mat mask_img(img.size(), CV_64F);
    generateGradient(mask_img, 1, 0.6);

    cv::Mat gradient;
    cv::normalize(mask_img, gradient, 0, 255, 32); //cv::normalize(maskImg, gradient, 0, 255, CV_MINMAX);

    cv::Mat lab_img(img.size(), CV_8UC3);
    // Change to CIE Lab color space
    cv::cvtColor(img, lab_img, cv::COLOR_BGR2Lab);

    for (int i = 0; i < lab_img.size().height; i++)
    {
        for (int j = 0; j < lab_img.size().width; j++)
        {
            cv::Vec3b value = lab_img.at<cv::Vec3b>(i, j);
            // value[0] = mask_img.at<double>(i, j);
            value.val[0] *= mask_img.at<double>(i, j);
            lab_img.at<cv::Vec3b>(i, j) = value;
        }
    }

    cv::Mat output;
    // Change to BGR
    cv::cvtColor(lab_img, output, cv::COLOR_Lab2BGR);

//    cv::imshow("Vignette", output);

    return output;
}
