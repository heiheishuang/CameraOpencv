//
// Created by heihei on 2020/5/14.
//

#include "FaceMakeup.h"
#include "utils.h"

#include <utility>


FaceMakeup::FaceMakeup()
= default;


FaceMakeup::~FaceMakeup()
= default;


cv::Mat FaceMakeup::SmoothImage(cv::Mat &img, int value_1, double value_2)
{

    cv::Mat smooth_face;
    int dx = value_1 * 5;
    double fc = value_1 * 12.5;

    int alpha = 50;

    cv::Mat temp_1, temp_2, temp_3, temp_4;
    cv::bilateralFilter(img, temp_1, dx, fc, fc);

    temp_2 = temp_1 - img + 128;

    cv::GaussianBlur(temp_2, temp_3, cv::Size(2 * value_2 - 1, 2 * value_2 - 1), 0, 0);

    temp_4 = img + 2 * temp_3 - 255;

    smooth_face = (img * (100 - alpha) + temp_4 * alpha) / 100;

    cv::imshow("Smoother", smooth_face);

    return smooth_face;

}


cv::Mat FaceMakeup::SmoothFace(cv::Mat &img, int value_1, double value_2, const cv::Rect &face_rect)
{
    cv::Mat temp_img, smooth_img;
    img.copyTo(temp_img);
    img.copyTo(smooth_img);
    smooth_img = SmoothImage(smooth_img, value_1, value_2);
    // temp_img = addAlphaChannel(temp_img);

    for (int i = 0; i < img.cols; i++)
    {
        for (int j = 0; j < img.rows; j++)
        {
            if (isInRect(i, j, face_rect))
            {
                temp_img.at<cv::Vec3b>(j, i) = smooth_img.at<cv::Vec3b>(j, i);
            }
        }
    }

//    cv::imshow("SmoothFace", temp_img);
    return temp_img;

}


bool FaceMakeup::isInRect(int i, int j, const cv::Rect &rect)
{
    return (i > rect.x and i < rect.x + rect.width) and (j > rect.y and j < rect.y + rect.height);
}


cv::Mat FaceMakeup::addAlphaChannel(cv::Mat &img)
{
    cv::Mat src = img.clone();
    cv::Mat alpha = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            alpha.at<uchar>(i, j) = 255;
        }
    }

    if (src.channels() == 4)
    {
        return src;
    }
    else if (src.channels() == 1)
    {
        cv::cvtColor(src, src, cv::COLOR_GRAY2RGB);
    }

    cv::Mat dst = cv::Mat(src.rows, src.cols, CV_8UC4);

    std::vector<cv::Mat> srcChannels;
    std::vector<cv::Mat> dstChannels;

    cv::split(src, srcChannels);

    dstChannels.push_back(srcChannels[0]);
    dstChannels.push_back(srcChannels[1]);
    dstChannels.push_back(srcChannels[2]);

    dstChannels.push_back(alpha);

    cv::merge(dstChannels, dst);

    // cv::imshow("After add alpha channel", dst);

    return dst;
}


bool FaceMakeup::isInLightSkin(cv::Mat &img, int i, int j)
{

    int h = 0;
    int s = 1;
    int v = 2;

    auto* p_src = img.ptr<uchar>(i, j);
    return (p_src[h] >= 0 and p_src[h] <= 20 and p_src[s] >= 48 and p_src[v] >= 50);
}


cv::Mat FaceMakeup::LightSkin(cv::Mat &src, double value_1, double value_2)
{
    cv::Mat img = src.clone();
    cv::Mat hsv_image;
    cvtColor(img, hsv_image, CV_BGR2HSV);

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            if (isInLightSkin(hsv_image, i, j))
            {

                for (int channel = 0; channel < 3; channel++)
                {
                    img.at<cv::Vec3b>(i, j)[channel] = cv::saturate_cast<uchar>(
                            value_1 * (img.at<cv::Vec3b>(i, j)[channel]) + value_2);
                }

            }
            else
            {
                // img.at<cv::Vec3b>(i, j)[0] = 0;
                // img.at<cv::Vec3b>(i, j)[1] = 0;
                // img.at<cv::Vec3b>(i, j)[2] = 0;

            }
        }
    }
//    cv::imshow("LinghtSkin", img);

    return img;
}


cv::Mat FaceMakeup::LightSkin(cv::Mat &img, double value_1, double value_2, const cv::Rect &face_rect)
{
    cv::Mat temp_img;
    img.copyTo(temp_img);
    // temp_img = addAlphaChannel(temp_img);

    for (int i = 0; i < img.cols; i++)
    {
        for (int j = 0; j < img.rows; j++)
        {
            if (isInRect(i, j, face_rect))
            {
                for (int channel = 0; channel < 3; channel++)
                {
                    double tmp = cv::saturate_cast<uchar>(
                            value_1 * (temp_img.at<cv::Vec3b>(j, i)[channel]) + value_2);
                    temp_img.at<cv::Vec3b>(j, i)[channel] = cv::saturate_cast<uchar>(
                            value_1 * (temp_img.at<cv::Vec3b>(j, i)[channel]) + value_2);
                }
            }
        }
    }

//    cv::imshow("LightSkin", temp_img);
    return temp_img;
}


cv::Mat FaceMakeup::LightSkin(cv::Mat &img, double value_1, double value_2, const std::vector<cv::Point> &face_point)
{
    cv::Mat src = img.clone();

    cv::Mat face_img = addAlphaChannel(src);

    face_img = getAllFace(face_img, face_point);

    return LightFace(img, face_img, value_1, value_2);
}


cv::Mat FaceMakeup::hsvSkinDetector(cv::Mat &src)
{
    cv::Mat hsv_image;
    int h = 0;
    int s = 1;
    int v = 2;
    // Transform the format of image
    cvtColor(src, hsv_image, CV_BGR2HSV);
    cv::Mat output_mask = cv::Mat::zeros(src.size(), CV_8UC1);
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            auto* p_mask = output_mask.ptr<uchar>(i, j);
            auto* p_src = hsv_image.ptr<uchar>(i, j);
            if (p_src[h] >= 0 and p_src[h] <= 20 and p_src[s] >= 48 and p_src[v] >= 50)
            {
                p_mask[0] = 255;
            }
        }
    }
    cv::Mat detect;
    src.copyTo(detect, output_mask);
    // imshow("HSV", detect);
    return detect;
}


cv::Mat FaceMakeup::getUpperLip(cv::Mat &img, std::vector<cv::Point> mouth_point)
{
    cv::Mat mouth_img;
    mouth_img = img.clone();

    // std::vector<cv::Point> lap_up_down;
    // getStraightLineAllPoint(mouth_point[0].x, mouth_point[0].y, mouth_point[12].x, mouth_point[12].y, lap_up_down);
    // getStraightLineAllPoint(mouth_point[12].x, mouth_point[12].y, mouth_point[13].x, mouth_point[13].y, lap_up_down);
    // getStraightLineAllPoint(mouth_point[13].x, mouth_point[13].y, mouth_point[14].x, mouth_point[14].y, lap_up_down);
    // getStraightLineAllPoint(mouth_point[14].x, mouth_point[14].y, mouth_point[15].x, mouth_point[15].y, lap_up_down);
    // getStraightLineAllPoint(mouth_point[15].x, mouth_point[15].y, mouth_point[16].x, mouth_point[16].y, lap_up_down);
    // getStraightLineAllPoint(mouth_point[16].x, mouth_point[16].y, mouth_point[6].x, mouth_point[6].y, lap_up_down);
    //
    // std::vector<cv::Point> lap_up_up;
    // getStraightLineAllPoint(mouth_point[0].x, mouth_point[0].y, mouth_point[1].x, mouth_point[1].y, lap_up_up);
    // getStraightLineAllPoint(mouth_point[1].x, mouth_point[1].y, mouth_point[2].x, mouth_point[2].y, lap_up_up);
    // getStraightLineAllPoint(mouth_point[2].x, mouth_point[2].y, mouth_point[3].x, mouth_point[3].y, lap_up_up);
    // getStraightLineAllPoint(mouth_point[3].x, mouth_point[3].y, mouth_point[4].x, mouth_point[4].y, lap_up_up);
    // getStraightLineAllPoint(mouth_point[4].x, mouth_point[4].y, mouth_point[5].x, mouth_point[5].y, lap_up_up);
    // getStraightLineAllPoint(mouth_point[5].x, mouth_point[5].y, mouth_point[6].x, mouth_point[6].y, lap_up_up);

    cv::Point rookPoints[1][12];
    rookPoints[0][0] = cv::Point(mouth_point[0].x, mouth_point[0].y);
    rookPoints[0][1] = cv::Point(mouth_point[1].x, mouth_point[1].y);
    rookPoints[0][2] = cv::Point(mouth_point[2].x, mouth_point[2].y);
    rookPoints[0][3] = cv::Point(mouth_point[3].x, mouth_point[3].y);
    rookPoints[0][4] = cv::Point(mouth_point[4].x, mouth_point[4].y);
    rookPoints[0][5] = cv::Point(mouth_point[5].x, mouth_point[5].y);
    rookPoints[0][6] = cv::Point(mouth_point[6].x, mouth_point[6].y);
    rookPoints[0][7] = cv::Point(mouth_point[16].x, mouth_point[16].y);
    rookPoints[0][8] = cv::Point(mouth_point[15].x, mouth_point[15].y);
    rookPoints[0][9] = cv::Point(mouth_point[14].x, mouth_point[14].y);
    rookPoints[0][10] = cv::Point(mouth_point[13].x, mouth_point[13].y);
    rookPoints[0][11] = cv::Point(mouth_point[12].x, mouth_point[12].y);

    const cv::Point* ppt[1] = {rookPoints[0]};
    int npt[] = {12};
    fillPoly(mouth_img, ppt, npt, 1, cv::Scalar(0, 0, 255, 0));
    // cv::imshow("Upper Lip", mouth_img);

    return mouth_img;

}


void FaceMakeup::getStraightLineAllPoint(int x0, int y0, int x1, int y1, std::vector<cv::Point> &points)
{
    int distance = 0;
    int xIncrement = 0;
    int yIncrement = 0;
    if (x0 == x1)
    {
        distance = y0 - y1;
        yIncrement = y0 > y1 ? -1 : 1;
    }
    else if (y0 == y1)
    {
        distance = x0 - x1;
        xIncrement = x0 > x1 ? -1 : 1;
    }

    for (int i = 0; i < std::abs(distance); i++)
    {
        points.emplace_back(x0 + xIncrement * i, y0 + yIncrement * i);
    }
}


cv::Mat FaceMakeup::getLowerLip(cv::Mat &img, std::vector<cv::Point> mouth_point)
{
    cv::Mat mouth_img;
    mouth_img = img.clone();

    // std::vector<cv::Point> lap_down_down;
    // getStraightLineAllPoint(mouth_point[0].x, mouth_point[0].y, mouth_point[11].x, mouth_point[11].y, lap_down_down);
    // getStraightLineAllPoint(mouth_point[11].x, mouth_point[11].y, mouth_point[10].x, mouth_point[10].y, lap_down_down);
    // getStraightLineAllPoint(mouth_point[10].x, mouth_point[10].y, mouth_point[9].x, mouth_point[9].y, lap_down_down);
    // getStraightLineAllPoint(mouth_point[9].x, mouth_point[9].y, mouth_point[8].x, mouth_point[8].y, lap_down_down);
    // getStraightLineAllPoint(mouth_point[8].x, mouth_point[8].y, mouth_point[7].x, mouth_point[7].y, lap_down_down);
    // getStraightLineAllPoint(mouth_point[7].x, mouth_point[7].y, mouth_point[6].x, mouth_point[6].y, lap_down_down);
    //
    // std::vector<cv::Point> lap_down_up;
    // getStraightLineAllPoint(mouth_point[0].x, mouth_point[0].y, mouth_point[12].x, mouth_point[12].y, lap_down_up);
    // getStraightLineAllPoint(mouth_point[12].x, mouth_point[12].y, mouth_point[19].x, mouth_point[19].y, lap_down_up);
    // getStraightLineAllPoint(mouth_point[19].x, mouth_point[19].y, mouth_point[18].x, mouth_point[18].y, lap_down_up);
    // getStraightLineAllPoint(mouth_point[18].x, mouth_point[18].y, mouth_point[17].x, mouth_point[17].y, lap_down_up);
    // getStraightLineAllPoint(mouth_point[17].x, mouth_point[17].y, mouth_point[16].x, mouth_point[16].y, lap_down_up);
    // getStraightLineAllPoint(mouth_point[16].x, mouth_point[16].y, mouth_point[6].x, mouth_point[6].y, lap_down_up);

    cv::Point rookPoints[1][12];
    rookPoints[0][0] = cv::Point(mouth_point[0].x, mouth_point[0].y);
    rookPoints[0][1] = cv::Point(mouth_point[12].x, mouth_point[12].y);
    rookPoints[0][2] = cv::Point(mouth_point[19].x, mouth_point[19].y);
    rookPoints[0][3] = cv::Point(mouth_point[18].x, mouth_point[18].y);
    rookPoints[0][4] = cv::Point(mouth_point[17].x, mouth_point[17].y);
    rookPoints[0][5] = cv::Point(mouth_point[16].x, mouth_point[16].y);
    rookPoints[0][6] = cv::Point(mouth_point[6].x, mouth_point[6].y);
    rookPoints[0][7] = cv::Point(mouth_point[7].x, mouth_point[7].y);
    rookPoints[0][8] = cv::Point(mouth_point[8].x, mouth_point[8].y);
    rookPoints[0][9] = cv::Point(mouth_point[9].x, mouth_point[9].y);
    rookPoints[0][10] = cv::Point(mouth_point[10].x, mouth_point[10].y);
    rookPoints[0][11] = cv::Point(mouth_point[11].x, mouth_point[11].y);


    const cv::Point* ppt[1] = {rookPoints[0]};
    int npt[] = {12};
    fillPoly(mouth_img, ppt, npt, 1, cv::Scalar(0, 0, 255, 0));
    // cv::imshow("Down Lip", mouth_img);

    return mouth_img;
}


cv::Mat FaceMakeup::getAllFace(cv::Mat &img, std::vector<cv::Point> face_point)
{
    cv::Mat temp_img;
    cv::Mat face_img;
    temp_img = img.clone();
    face_img = img.clone();

    // cv::Point rookPoints[1][27];
    // rookPoints[0][0] = cv::Point(face_point[0].x, face_point[0].y);
    // rookPoints[0][1] = cv::Point(face_point[1].x, face_point[1].y);
    // rookPoints[0][2] = cv::Point(face_point[2].x, face_point[2].y);
    // rookPoints[0][3] = cv::Point(face_point[3].x, face_point[3].y);
    // rookPoints[0][4] = cv::Point(face_point[4].x, face_point[4].y);
    // rookPoints[0][5] = cv::Point(face_point[5].x, face_point[5].y);
    // rookPoints[0][6] = cv::Point(face_point[6].x, face_point[6].y);
    // rookPoints[0][7] = cv::Point(face_point[7].x, face_point[7].y);
    // rookPoints[0][8] = cv::Point(face_point[8].x, face_point[8].y);
    // rookPoints[0][9] = cv::Point(face_point[9].x, face_point[9].y);
    // rookPoints[0][10] = cv::Point(face_point[10].x, face_point[10].y);
    // rookPoints[0][11] = cv::Point(face_point[11].x, face_point[11].y);
    // rookPoints[0][12] = cv::Point(face_point[12].x, face_point[12].y);
    // rookPoints[0][13] = cv::Point(face_point[13].x, face_point[13].y);
    // rookPoints[0][14] = cv::Point(face_point[14].x, face_point[14].y);
    // rookPoints[0][15] = cv::Point(face_point[15].x, face_point[15].y);
    // rookPoints[0][16] = cv::Point(face_point[16].x, face_point[16].y);
    // rookPoints[0][17] = cv::Point(face_point[26].x, face_point[26].y);
    // rookPoints[0][18] = cv::Point(face_point[25].x, face_point[25].y);
    // rookPoints[0][19] = cv::Point(face_point[24].x, face_point[24].y);
    // rookPoints[0][20] = cv::Point(face_point[23].x, face_point[23].y);
    // rookPoints[0][21] = cv::Point(face_point[22].x, face_point[22].y);
    // rookPoints[0][22] = cv::Point(face_point[21].x, face_point[21].y);
    // rookPoints[0][23] = cv::Point(face_point[20].x, face_point[20].y);
    // rookPoints[0][24] = cv::Point(face_point[19].x, face_point[19].y);
    // rookPoints[0][25] = cv::Point(face_point[18].x, face_point[18].y);
    // rookPoints[0][26] = cv::Point(face_point[17].x, face_point[17].y);

    cv::Point rookPoints[1][19];
    rookPoints[0][0] = cv::Point(face_point[0].x, face_point[0].y);
    rookPoints[0][1] = cv::Point(face_point[1].x, face_point[1].y);
    rookPoints[0][2] = cv::Point(face_point[2].x, face_point[2].y);
    rookPoints[0][3] = cv::Point(face_point[3].x, face_point[3].y);
    rookPoints[0][4] = cv::Point(face_point[4].x, face_point[4].y);
    rookPoints[0][5] = cv::Point(face_point[5].x, face_point[5].y);
    rookPoints[0][6] = cv::Point(face_point[6].x, face_point[6].y);
    rookPoints[0][7] = cv::Point(face_point[7].x, face_point[7].y);
    rookPoints[0][8] = cv::Point(face_point[8].x, face_point[8].y);
    rookPoints[0][9] = cv::Point(face_point[9].x, face_point[9].y);
    rookPoints[0][10] = cv::Point(face_point[10].x, face_point[10].y);
    rookPoints[0][11] = cv::Point(face_point[11].x, face_point[11].y);
    rookPoints[0][12] = cv::Point(face_point[12].x, face_point[12].y);
    rookPoints[0][13] = cv::Point(face_point[13].x, face_point[13].y);
    rookPoints[0][14] = cv::Point(face_point[14].x, face_point[14].y);
    rookPoints[0][15] = cv::Point(face_point[15].x, face_point[15].y);
    rookPoints[0][16] = cv::Point(face_point[16].x, face_point[16].y);
    rookPoints[0][17] = cv::Point(face_point[24].x, face_point[24].y);
    rookPoints[0][18] = cv::Point(face_point[19].x, face_point[19].y);

    const cv::Point* ppt[1] = {rookPoints[0]};
    int npt[] = {19};
    fillPoly(temp_img, ppt, npt, 1, cv::Scalar(255, 255, 255, 0));

    // cv::imshow("All Face1", temp_img);

    for (int i = 0; i < temp_img.cols; i++)
    {
        for (int j = 0; j < temp_img.rows; j++)
        {
            if (temp_img.at<cv::Vec3b>(i, j)[0] == 255 and temp_img.at<cv::Vec3b>(i, j)[1] == 255
                and temp_img.at<cv::Vec3b>(i, j)[2] == 255 and temp_img.at<cv::Vec3b>(i, j)[3] == 0)
            {
                face_img.at<cv::Vec3b>(i, j)[3] = 0;
                face_img.at<cv::Vec3b>(i, j)[0] = face_img.at<cv::Vec3b>(i, j)[0];
                face_img.at<cv::Vec3b>(i, j)[1] = face_img.at<cv::Vec3b>(i, j)[1];
                face_img.at<cv::Vec3b>(i, j)[2] = face_img.at<cv::Vec3b>(i, j)[2];
            }
        }
    }
//    cv::imshow("All Face", face_img);

    return face_img;
}


cv::Mat FaceMakeup::getRightEye(cv::Mat &img, std::vector<cv::Point> eye_point)
{
    cv::Mat eye_img;
    eye_img = img.clone();

    cv::Point rookPoints[1][6];
    rookPoints[0][0] = cv::Point(eye_point[36].x, eye_point[36].y);
    rookPoints[0][1] = cv::Point(eye_point[37].x, eye_point[37].y);
    rookPoints[0][2] = cv::Point(eye_point[38].x, eye_point[38].y);
    rookPoints[0][3] = cv::Point(eye_point[39].x, eye_point[39].y);
    rookPoints[0][4] = cv::Point(eye_point[40].x, eye_point[40].y);
    rookPoints[0][5] = cv::Point(eye_point[41].x, eye_point[41].y);

    const cv::Point* ppt[1] = {rookPoints[0]};
    int npt[] = {6};
    fillPoly(eye_img, ppt, npt, 1, cv::Scalar(0, 0, 255, 0));
    // cv::imshow("Right Eye", eye_img);

    return eye_img;
}


cv::Mat FaceMakeup::getLeftEye(cv::Mat &img, std::vector<cv::Point> eye_point)
{
    cv::Mat eye_img;
    eye_img = img.clone();

    cv::Point rookPoints[1][6];
    rookPoints[0][0] = cv::Point(eye_point[42].x, eye_point[42].y);
    rookPoints[0][1] = cv::Point(eye_point[43].x, eye_point[43].y);
    rookPoints[0][2] = cv::Point(eye_point[44].x, eye_point[44].y);
    rookPoints[0][3] = cv::Point(eye_point[45].x, eye_point[45].y);
    rookPoints[0][4] = cv::Point(eye_point[46].x, eye_point[46].y);
    rookPoints[0][5] = cv::Point(eye_point[47].x, eye_point[47].y);

    const cv::Point* ppt[1] = {rookPoints[0]};
    int npt[] = {6};
    fillPoly(eye_img, ppt, npt, 1, cv::Scalar(0, 0, 255, 0));
    // cv::imshow("Left Eye", eye_img);

    return eye_img;
}


cv::Mat FaceMakeup::createAlpha(cv::Mat &img)
{
    cv::Mat src = img.clone();
    cv::Mat alpha = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
    cv::Mat gray = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

    cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            // alpha.at<uchar>(i, j) = 255 - gray.at<uchar>(i, j);
            alpha.at<uchar>(i, j) = 100;
        }
    }

    return alpha;
}


cv::Mat FaceMakeup::addAlphaChannel(cv::Mat &img, cv::Mat &alpha)
{
    cv::Mat src = img.clone();

    if (src.channels() == 4)
    {
        return src.clone();
    }
    else if (src.channels() == 1)
    {
        cv::cvtColor(src, src, cv::COLOR_GRAY2RGB);
    }

    cv::Mat dst = cv::Mat(src.rows, src.cols, CV_8UC4);

    std::vector<cv::Mat> srcChannels;
    std::vector<cv::Mat> dstChannels;

    cv::split(src, srcChannels);

    dstChannels.push_back(srcChannels[0]);
    dstChannels.push_back(srcChannels[1]);
    dstChannels.push_back(srcChannels[2]);

    dstChannels.push_back(alpha);

    cv::merge(dstChannels, dst);

    // imshow("add alpha wiht alpha mat", dst);
    return dst;

}


cv::Mat FaceMakeup::ColorImage(cv::Mat &img, int value)
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
            rgbMax = getMaxPixRgb(src.at<cv::Vec3b>(i, j)[0], src.at<cv::Vec3b>(i, j)[1], src.at<cv::Vec3b>(i, j)[2]);
            rgbMin = getMinPixRgb(src.at<cv::Vec3b>(i, j)[0], src.at<cv::Vec3b>(i, j)[1], src.at<cv::Vec3b>(i, j)[2]);

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

    imshow("Saturation Adjust Image", src);

    return src;

}


cv::Mat FaceMakeup::colorMouth(cv::Mat &face_img, cv::Mat &img, int value)
{
    // cv::Mat src(img.size(), CV_8UC3, cv::Scalar(255, 255, 255));

    cv::Mat src;
    if (img.channels() == 3)
    {
        std::cout << "Has Problem in image channels" << std::endl;
    }

    src = deleteAlphaChannel(img);


    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {

            if (img.at<cv::Vec4b>(i, j)[4] == 0)
            {
                cv::Vec3b new_pix(face_img.at<cv::Vec3b>(i, j)[0], face_img.at<cv::Vec3b>(i, j)[1],
                                  face_img.at<cv::Vec3b>(i, j)[2]);
                new_pix = adjustSaturationAdjust(new_pix, value);

                src.at<cv::Vec3b>(i, j)[0] = new_pix[0];
                src.at<cv::Vec3b>(i, j)[1] = new_pix[1];
                src.at<cv::Vec3b>(i, j)[2] = new_pix[2];
            }
        }
    }

//    imshow("colorMouth", src);
    return src;
}


cv::Vec3b FaceMakeup::adjustSaturationAdjust(const cv::Vec3b &pix, int value)
{
    uchar rgbMax = getMaxPixRgb(pix[0], pix[1], pix[2]);
    uchar rgbMin = getMinPixRgb(pix[0], pix[1], pix[2]);
    double dPercent = static_cast<double>(value) / 100;

    double dDelta;
    double dValue;
    double dL;
    double dS;
    double dAlpha;

    cv::Vec3b adjust_pix;

    dDelta = static_cast<double>(rgbMax - rgbMin) / 255;
    dValue = static_cast<double>(rgbMax + rgbMin) / 255;

    if (dDelta == 0)
    {
        return pix;
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

        adjust_pix[0] = pix[0] + (pix[0] - dL * 255) * dAlpha;
        adjust_pix[1] = pix[1] + (pix[1] - dL * 255) * dAlpha;
        adjust_pix[2] = pix[2] + (pix[2] - dL * 255) * dAlpha;
    }
    else
    {
        dAlpha = dPercent;
        adjust_pix[0] = dL * 255 + (pix[0] - dL * 255) * (1 + dAlpha);
        adjust_pix[1] = dL * 255 + (pix[1] - dL * 255) * (1 + dAlpha);
        adjust_pix[2] = dL * 255 + (pix[2] - dL * 255) * (1 + dAlpha);
    }

    return adjust_pix;
}


cv::Mat FaceMakeup::deleteAlphaChannel(cv::Mat &img)
{
    cv::Mat src = img.clone();

    if (src.channels() == 3)
    {
        return src;
    }
    else if (src.channels() == 1)
    {
        cv::cvtColor(src, src, cv::COLOR_GRAY2RGB);
    }

    cv::Mat dst = cv::Mat(src.rows, src.cols, CV_8UC3);

    std::vector<cv::Mat> srcChannels;
    std::vector<cv::Mat> dstChannels;

    cv::split(src, srcChannels);

    dstChannels.push_back(srcChannels[0]);
    dstChannels.push_back(srcChannels[1]);
    dstChannels.push_back(srcChannels[2]);

    cv::merge(dstChannels, dst);

    return dst;
}


cv::Mat FaceMakeup::RedMouth(cv::Mat &img, int value, const std::vector<cv::Point> &mouth_point)
{
    cv::Mat src = img.clone();

    cv::Mat mouth_img = addAlphaChannel(src);

    mouth_img = getUpperLip(mouth_img, mouth_point);
    mouth_img = getLowerLip(mouth_img, mouth_point);

    return colorMouth(img, mouth_img, value);


}


cv::Mat FaceMakeup::LightFace(cv::Mat &img, cv::Mat &face_img, double value_1, double value_2)
{
    cv::Mat src;
    if (face_img.channels() == 3)
    {
        std::cout << "Has Problem in image channels" << std::endl;
    }

    src = deleteAlphaChannel(face_img);
    // src = img.clone();

    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            if (face_img.at<cv::Vec4b>(i, j)[3] == 0)
            {
                // for (int channel = 0; channel < 3; channel++)
                // {
                //     src.at<cv::Vec3b>(i, j)[channel] = cv::saturate_cast<uchar>(
                //             value_1 * (img.at<cv::Vec3b>(i, j)[channel]) + value_2);
                // }
                // TODO
                // Maybe channel has problem
                src.at<cv::Vec3b>(i, j)[0] = cv::saturate_cast<uchar>(
                        value_1 * (img.at<cv::Vec3b>(i, j)[0]) + value_2);
                src.at<cv::Vec3b>(i, j)[1] = cv::saturate_cast<uchar>(
                        value_1 * (img.at<cv::Vec3b>(i, j)[1]) + value_2);
                src.at<cv::Vec3b>(i, j)[2] = cv::saturate_cast<uchar>(
                        value_1 * (img.at<cv::Vec3b>(i, j)[2]) + value_2);
            }

        }
    }

//    imshow("colorMouth", src);
    return src;

}
