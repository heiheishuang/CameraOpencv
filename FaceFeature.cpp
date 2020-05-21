//
// Created by heihei on 2020/5/13.
//

#include "FaceFeature.h"


FaceFeature::FaceFeature(cv::Mat &img)
{
    try
    {
        // Load face detection and pose estimation models.
        detector = dlib::get_frontal_face_detector();
        dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;

        if (!face_cascade.load("haarcascade_frontalface_alt.xml"))
        {
            std::cout << "Problem! Need to load haarcascade_frontalface_alt.xml!" << std::endl;
        }

    }
    catch (dlib::serialization_error &e)
    {
        std::cout << "You need dlib's default face landmarking model file to run this example." << std::endl;
        std::cout << "You can get it from the following URL: " << std::endl;
        std::cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << std::endl;
        std::cout << std::endl << e.what() << std::endl;
    }

    findFeature(img);

    findFace(img);

    face_img = img;

}


FaceFeature::~FaceFeature()
= default;


void FaceFeature::drawPolyline(cv::Mat &img, const dlib::full_object_detection &d, int start, int end, bool isClosed)
{
    std::vector<cv::Point> points;
    for (int i = start; i <= end; i++)
    {
        cv::Point point(d.part(i).x(), d.part(i).y());
        points.push_back(point);
    }

    cv::polylines(img, points, isClosed, cv::Scalar(255, 0, 0), 2, 16);
}


void FaceFeature::renderFace(cv::Mat &img, const dlib::full_object_detection &d)
{
    DLIB_CASSERT
    (
            d.num_parts() == 68,
            "\n\t Invalid inputs were given to this function. "
                    << "\n\t d.num_parts():  " << d.num_parts()
    );

    // Jaw line
    drawPolyline(img, d, 0, 16, false);
    // Left eyebrow
    drawPolyline(img, d, 17, 21, false);
    // Right eyebrow
    drawPolyline(img, d, 22, 26, false);
    // Nose bridge
    drawPolyline(img, d, 27, 30, false);
    // Lower nose
    drawPolyline(img, d, 30, 35, true);
    // Left eye
    drawPolyline(img, d, 36, 41, true);
    // Right Eye
    drawPolyline(img, d, 42, 47, true);
    // Outer lip
    drawPolyline(img, d, 48, 59, true);
    // Inner lip
    drawPolyline(img, d, 60, 67, true);

}


void FaceFeature::findFeature(cv::Mat &img)
{

    cv::Mat img_temp = img.clone();
    dlib::cv_image<dlib::bgr_pixel> cimg(img_temp);
    // Detect faces
    std::vector<dlib::rectangle> faces = detector(cimg);
    // Find the pose of each face.
    std::vector<dlib::full_object_detection> shapes;

    for (auto &face : faces)
    {
        dlib::full_object_detection shape = pose_model(cimg, face);
        shapes.push_back(shape);
        // Now Only use one face
        break;

        // for (int i = 0; i < 68; i++)
        // {
        //     circle(img_temp, cvPoint(shapes[count].part(i).x(), shapes[count].part(i).y()), 3, cv::Scalar(0, 0, 255), -1);
        //     cv::putText(img_temp,std::to_string(i),cvPoint(shapes[count].part(i).x(), shapes[count].part(i).y()),FONT_HERSHEY_COMPLEX,0.3,(0,0,255),1);
        // }

        // renderFace(img_temp, shape);

    }

    //
    // if (!shapes.empty())
    // {
    //     for (int i = 0; i < 68; i++)
    //     {
    //         circle(img_temp, cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 3, cv::Scalar(0, 0, 255), -1);
    //     }
    // }

    face_feature = shapes[0];
    // imshow("FaceFeature", img_temp);

}


cv::Mat FaceFeature::transformFacePoint(cv::Mat &img, double change, const cv::Rect &face)
{

    std::vector<cv::Point2f> landmarks;
    changeLandmarks(face_feature, landmarks);

    std::vector<cv::Point2f> control_p = {
                                            landmarks[0],
                                            landmarks[5],
                                            cv::Point2f(face.x, face.y + face.height),
                                            cv::Point2f(face.x + face.width,face.y + face.height),
                                            landmarks[11],
                                            landmarks[16],
                                            landmarks[33],
                                            landmarks[62]
    };
    std::vector<cv::Point2f> control_q = {
                                            landmarks[0],
                                            cv::Point2f(landmarks[5].x + change, landmarks[5].y),
                                            cv::Point2f(face.x,face.y + face.height),
                                            cv::Point2f(face.x + face.width, face.y + face.height),
                                            cv::Point2f(landmarks[11].x - change,landmarks[11].y),
                                            landmarks[16],
                                            landmarks[33],
                                            landmarks[62]
    };

    cv::Mat tf_img = img.clone();

    for (int i = face.x; i <= face.x + face.width; ++i)
    {

        for (int j = landmarks[0].y; j < landmarks[8].y; ++j)
        {

            cv::Mat pos;
            pos = transformPoint(control_p, control_q, i, j);

            int row = pos.at<float>(0, 0);
            int col = pos.at<float>(0, 1);

            // std::cout << col << " " << j << " " << row << " " << i <<std::endl;
            if (0 < col and col < img.cols and 0 < row and row < img.rows)
            {
                tf_img.at<cv::Vec3b>(col, row)[0] = img.at<cv::Vec3b>(j, i)[0];
                tf_img.at<cv::Vec3b>(col, row)[1] = img.at<cv::Vec3b>(j, i)[1];
                tf_img.at<cv::Vec3b>(col, row)[2] = img.at<cv::Vec3b>(j, i)[2];
            }

        }

    }

//    imshow("transformFacePoint", tf_img);
    return tf_img;
}


void FaceFeature::changeLandmarks(const dlib::full_object_detection &d, std::vector<cv::Point2f> &landmarks)
{
    if (landmarks.empty())
    {
        for (int i = 0; i < 68; i++)
        {
            cv::Point2f point(d.part(i).x(), d.part(i).y());
            landmarks.push_back(point);
        }
    }
    else
    {
        for (int i = 0; i < 68; i++)
        {
            cv::Point2f point(d.part(i).x(), d.part(i).y());
            landmarks[i] = point;
        }
    }
}


cv::Rect FaceFeature::findFace(cv::Mat &img)
{
    cv::Mat img_temp;
    cv::cvtColor(img, img_temp, CV_BGR2GRAY);
    equalizeHist(img_temp, img_temp);

    std::vector<cv::Rect> face_rects;

    face_cascade.detectMultiScale(img_temp, face_rects, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));

    // Use rectangle to draw the face
    // rectangle(img_temp, face_rect[0], Scalar(0, 0, 255));

    face_rect = face_rects[0];
    return face_rects[0];
}


cv::Mat FaceFeature::smallFace(double degree)
{
    return transformFacePoint(face_img, degree, face_rect);
}


cv::Mat FaceFeature::transformPoint(std::vector<cv::Point2f> control_p, std::vector<cv::Point2f> control_q, int img_i, int img_j)
{
    std::vector<float> weight_p;
    std::vector<cv::Point2f>::iterator itcp = control_p.begin();
    while (itcp != control_p.end())
    {
        double tmp;
        if (itcp->x != static_cast<float >(img_i) || itcp->y != static_cast<float >(img_j))
            tmp = 1 / ((itcp->x - static_cast<float >(img_i)) * (itcp->x - static_cast<float >(img_i))
                    + (itcp->y - static_cast<float >(img_j)) * (itcp->y - static_cast<float >(img_j)));
        else
            tmp = 32768; //TODO
        weight_p.push_back(tmp);
        ++itcp;
    }

    double px = 0, py = 0, qx = 0, qy = 0, tw = 0;
    itcp = control_p.begin();

    std::vector<float>::iterator itwp = weight_p.begin();
    std::vector<cv::Point2f>::iterator itcq = control_q.begin();
    while (itcp != control_p.end())
    {
        px += (*itwp) * (itcp->x);
        py += (*itwp) * (itcp->y);
        qx += (*itwp) * (itcq->x);
        qy += (*itwp) * (itcq->y);

        tw += *itwp;
        ++itcp;
        ++itcq;
        ++itwp;
    }
    px = px / tw;
    py = py / tw;
    qx = qx / tw;
    qy = qy / tw;

    cv::Mat A = cv::Mat::zeros(2, 1, CV_32FC1);
    cv::Mat B = cv::Mat::zeros(1, 2, CV_32FC1);
    cv::Mat C = cv::Mat::zeros(1, 2, CV_32FC1);
    cv::Mat sumL = cv::Mat::zeros(2, 2, CV_32FC1);
    cv::Mat sumR = cv::Mat::zeros(2, 2, CV_32FC1);
    cv::Mat M, pos;
    for (int index = 0; index < weight_p.size(); ++index)
    {
        A.at<float>(0, 0) = (control_p[index].x - px);
        A.at<float>(1, 0) = (control_p[index].y - py);
        B.at<float>(0, 0) = weight_p[index] * ((control_p[index].x - px));
        B.at<float>(0, 1) = weight_p[index] * ((control_p[index].y - py));
        sumL += A * B;

        C.at<float>(0, 0) = weight_p[index] * (control_q[index].x - qx);
        C.at<float>(0, 1) = weight_p[index] * (control_q[index].y - qy);
        sumR += A * C;
    }
    M = sumL.inv() * sumR;

    B.at<float>(0, 0) = img_i - px;
    B.at<float>(0, 1) = img_j - py;
    C.at<float>(0, 0) = qx;
    C.at<float>(0, 1) = qy;
    pos = B * M + C;

    return pos;
}


cv::Mat FaceFeature::bigEye(double degree)
{
    return transformEyePoint(face_img, degree, face_rect);
}


cv::Mat FaceFeature::transformEyePoint(cv::Mat &img, double change, const cv::Rect &face)
{

    std::vector<cv::Point2f> landmarks;
    changeLandmarks(face_feature, landmarks);

    std::vector<cv::Point2f> control_p = {
            landmarks[0],
            landmarks[36],
            cv::Point2f(face.x, face.y + face.height),
            cv::Point2f(face.x + face.width,face.y + face.height),
            landmarks[45],
            landmarks[37],
            landmarks[38],
            landmarks[40],
            landmarks[41],
            landmarks[43],
            landmarks[44],
            landmarks[46],
            landmarks[47]
    };
    std::vector<cv::Point2f> control_q = {
            landmarks[0],
            cv::Point2f(landmarks[36].x - change, landmarks[36].y),
            cv::Point2f(face.x,face.y + face.height),
            cv::Point2f(face.x + face.width, face.y + face.height),
            cv::Point2f(landmarks[45].x + change,landmarks[45].y),
            cv::Point2f(landmarks[37].x, landmarks[37].y - change),
            cv::Point2f(landmarks[38].x, landmarks[38].y - change),
            cv::Point2f(landmarks[40].x, landmarks[40].y + change),
            cv::Point2f(landmarks[41].x, landmarks[41].y + change),
            cv::Point2f(landmarks[43].x, landmarks[43].y - change),
            cv::Point2f(landmarks[44].x, landmarks[44].y - change),
            cv::Point2f(landmarks[46].x, landmarks[46].y + change),
            cv::Point2f(landmarks[47].x, landmarks[47].y + change)
    };

    cv::Mat tf_img = img.clone();

    for (int i = face.x; i <= face.x + face.width * 4 / 3; ++i)
    {

        for (int j = landmarks[19].y; j < landmarks[3].y; ++j)
        {

            cv::Mat pos;
            pos = transformPoint(control_p, control_q, i, j);

            int row = pos.at<float>(0, 0);
            int col = pos.at<float>(0, 1);

            // std::cout << col << " " << j << " " << row << " " << i <<std::endl;
            if (0 < col and col < img.cols and 0 < row and row < img.rows)
            {
                tf_img.at<cv::Vec3b>(col, row)[0] = img.at<cv::Vec3b>(j, i)[0];
                tf_img.at<cv::Vec3b>(col, row)[1] = img.at<cv::Vec3b>(j, i)[1];
                tf_img.at<cv::Vec3b>(col, row)[2] = img.at<cv::Vec3b>(j, i)[2];
            }

        }

    }

//    imshow("transformEyePoint", tf_img);
    return tf_img;
}


void FaceFeature::setNewImage(cv::Mat &img)
{
    face_img = img;

    findFeature(img);

    findFace(img);

}


cv::Mat FaceFeature::smoothFace(int value_1, double value_2)
{
    return FaceMakeup::SmoothFace(face_img, value_1, value_2, face_rect);
}


cv::Mat FaceFeature::LightFace(int model, double value_1, double value_2)
{
    // return FaceMakeup::LightSkin(face_img, value_1, value_2);

    if (model == 1)
    {
        cv::Rect new_face;
        new_face = face_rect;
        new_face.y = face_feature.part(37).y();
        new_face.height = face_feature.part(8).y() - new_face.y;
        return FaceMakeup::LightSkin(face_img, value_1, value_2, new_face);
    }
    else if (model == 2)
    {
        return FaceMakeup::LightSkin(face_img, value_1, value_2);
    }
    else if (model == 3)
    {
        std::vector<cv::Point> rookPoints;
        for (int i = 0; i < 26; i++)
        {
            cv::Point point(face_feature.part(i).x(), face_feature.part(i).y());
            rookPoints.push_back(point);
        }

        return FaceMakeup::LightSkin(face_img, value_1, value_2, rookPoints);
    }

}


cv::Mat FaceFeature::redMouth(int value)
{
    std::vector<cv::Point> rookPoints;
    for (int i = 48; i < 68; i++)
    {
        cv::Point point(face_feature.part(i).x(), face_feature.part(i).y());
        rookPoints.push_back(point);
    }

    return FaceMakeup::RedMouth(face_img, value, rookPoints);
    //
    // return FaceMakeup::ColorImage(face_img, value);


}


