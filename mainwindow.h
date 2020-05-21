#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QSlider>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <opencv2/opencv.hpp>

#include "ImageFilter.h"
#include "FaceFeature.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    cv::Mat changeQImage2Mat(QImage img);

    QImage changeMat2QImage(cv::Mat img);

    void outputImage(cv::Mat img);

    bool isHasImage();

private slots:
    void on_pushButton_Open_clicked();

    void on_pushButton_OpenCamera_clicked();

    void on_pushButton_Close_clicked();

    void on_pushButton_Save_As_clicked();

    void on_pushButton_FilterN_clicked();

    void on_pushButton_FilterC_clicked();

    void on_pushButton_FilterE_clicked();

    void showFilterE();

    void on_pushButton_FilterSa_clicked();

    void showFilterSa();

    void on_pushButton_FilterG_clicked();

    void showFilterG();

    void on_pushButton_FilterD_clicked();

    void showFilterD();

    void on_pushButton_FilterSh_clicked();

    void showFilterSh();

    void on_pushButton_FilterV_clicked();

    void showFilterV();

    void on_pushButton_FaceBig_clicked();

    void on_pushButton_FaceSmall_clicked();

    void on_pushButton_MakeUpLight_clicked();

    void showFaceLight();

    void on_pushButton_MakeUpRed_clicked();

    void showFaceRed();

    void on_pushButton_MakeUpSmooth_clicked();

    void showFaceSmooth();

    void on_pushButton_Restore_clicked();

private:
    Ui::MainWindow *ui;

    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;

    cv::VideoCapture capture;

    QSpinBox *spinBox_1;
    QSlider *slider_1;
    QSpinBox *spinBox_2;
    QSlider *slider_2;
    QSpinBox *spinBox_3;
    QSlider *slider_3;
    QPushButton *pushButton;
    QWidget *small_window;

    cv::Mat src;
    cv::Mat res;
    cv::Mat tmp;
};
#endif // MAINWINDOW_H
