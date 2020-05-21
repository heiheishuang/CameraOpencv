#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->LabelImage->setText("Please input the Image!");

    //Big Eye
    ui->horizontalSliderBig->setRange(0, 10);
    ui->spinBoxBig->setRange(0, 10);

    QObject::connect(ui->horizontalSliderBig, SIGNAL(valueChanged(int)), ui->spinBoxBig, SLOT(setValue(int)));
    QObject::connect(ui->spinBoxBig, SIGNAL(valueChanged(int)), ui->horizontalSliderBig, SLOT(setValue(int)));
    ui->spinBoxBig->setValue(2);

    //Red Mouth
    ui->horizontalSliderSmall->setRange(0, 10);
    ui->spinBoxSmall->setRange(0, 10);

    QObject::connect(ui->horizontalSliderSmall, SIGNAL(valueChanged(int)), ui->spinBoxSmall, SLOT(setValue(int)));
    QObject::connect(ui->spinBoxSmall, SIGNAL(valueChanged(int)), ui->horizontalSliderSmall, SLOT(setValue(int)));
    ui->spinBoxSmall->setValue(5);
}


MainWindow::~MainWindow()
{
    delete ui;
}


cv::Mat MainWindow::changeQImage2Mat(QImage img)
{
    cv::Mat src;
    src = cv::Mat(img.height(), img.width(),
                  CV_8UC3,(void*)img.constBits(),img.bytesPerLine());

    return src;
}


QImage MainWindow::changeMat2QImage(cv::Mat img)
{
    cv::Mat src = img.clone();
    cv::cvtColor(src, src, CV_BGR2RGB);
    QImage q_image;
    q_image = QImage((const uchar*)(src.data), src.cols, src.rows,
                     src.cols * src.channels(), QImage::Format_RGB888);

    return q_image;
}


void MainWindow::outputImage(cv::Mat img)
{
    cv::Mat res_copy = img.clone();

    if (img.empty())
    {
        ui->LabelImage->setText("The result is empty!");
        return;
    }

    QImage img_output;
    if (res_copy.channels() == 3)
    {
        cv::Mat hei = res_copy.clone();
        cv::cvtColor(res_copy, res_copy, CV_BGR2RGB);

        img_output = QImage((const uchar*)(res_copy.data), res_copy.cols, res_copy.rows,
                     res_copy.cols * res_copy.channels(), QImage::Format_RGB888);
    }
    else
    {
        img_output = QImage((const uchar*)(res_copy.data), res_copy.cols, res_copy.rows,
                     res_copy.cols * res_copy.channels(), QImage::Format_Indexed8);
    }

    ui->LabelImage->setPixmap( QPixmap::fromImage(img_output));

    ui->LabelImage->resize( ui->LabelImage->pixmap()->size());
}


bool MainWindow::isHasImage()
{
    if (src.empty())
    {
        return false;
    }
    else
    {
        return true;
    }
}


void MainWindow::showFilterE()
{
    double value = spinBox_1->value();

    if (src.empty())
    {
        ui->LabelImage->setText("Please open the image or open the camera!");
        return;
    }

//    cv::imshow("Filter Emergence", src);
    res = ImageFilter::FilterEmergence(src, value / 10);
//    cv::imshow("after", res);
    cv::Mat res_copy = res.clone();

    outputImage(res);
}


void MainWindow::showFilterG()
{
    double value = spinBox_1->value();

    if (src.empty())
    {
        ui->LabelImage->setText("Please open the image or open the camera!");
        return;
    }

//    cv::imshow("Filter Ground Glass", src);
    res = ImageFilter::FilterGroundGlass(src, value);
//    cv::imshow("after", res);
    cv::Mat res_copy = res.clone();

    outputImage(res);
}


void MainWindow::showFilterSa()
{
    double value = spinBox_1->value();

    if (src.empty())
    {
        ui->LabelImage->setText("Please open the image or open the camera!");
        return;
    }

//    cv::imshow("Filter Saturation", src);
    res = ImageFilter::FilterSaturation(src, value);
//    cv::imshow("after", res);
    cv::Mat res_copy = res.clone();

    outputImage(res);
}


void MainWindow::showFilterD()
{
    double value_1 = spinBox_1->value();
    value_1 = value_1 / 10;

    double value_2 = spinBox_2->value();

    if (src.empty())
    {
        ui->LabelImage->setText("Please open the image or open the camera!");
        return;
    }

    //    cv::imshow("Filter Distinctive", src);
    res = ImageFilter::FilterDistinctive(src, value_1, value_2);
    //    cv::imshow("after", res);
    cv::Mat res_copy = res.clone();

    outputImage(res);
}


void MainWindow::showFilterSh()
{
    double value_1 = spinBox_1->value();
    value_1 = value_1 / 10;

    double value_2 = spinBox_2->value();
    value_2 = value_2 / 10;

    if (src.empty())
    {
        ui->LabelImage->setText("Please open the image or open the camera!");
        return;
    }

    //    cv::imshow("Filter Sharpen", src);
    res = ImageFilter::FilterSharpen(src, value_1, value_2);
    //    cv::imshow("after", res);
    cv::Mat res_copy = res.clone();

    outputImage(res);
}


void MainWindow::showFilterV()
{
    double value_1 = spinBox_1->value();
    value_1 = value_1 / 10;

    double value_2 = spinBox_2->value();
    value_2 = value_2 / 10;

    if (src.empty())
    {
        ui->LabelImage->setText("Please open the image or open the camera!");
        return;
    }

    //    cv::imshow("Filter Vignetting", src);
    res = ImageFilter::FilterVignetting(src, value_1, value_2);
    //    cv::imshow("after", res);
    cv::Mat res_copy = res.clone();

    outputImage(res);
}


void MainWindow::showFaceLight()
{
    double value_1 = spinBox_1->value();

    double value_2 = spinBox_2->value();
    value_2 = value_2 / 10;

    double value_3 = spinBox_2->value();

    if (src.empty())
    {
        ui->LabelImage->setText("Please open the image or open the camera!");
        return;
    }

    //    cv::imshow("Filter Vignetting", src);
    FaceFeature face_feature(src);
    res = face_feature.LightFace(value_1, value_2, value_3);
    //    cv::imshow("after", res);
    cv::Mat res_copy = res.clone();

    outputImage(res);
}


void MainWindow::showFaceRed()
{
    double value_1 = spinBox_1->value();

    if (src.empty())
    {
        ui->LabelImage->setText("Please open the image or open the camera!");
        return;
    }

    //    cv::imshow("Filter Vignetting", src);
    FaceFeature face_feature(src);
    res = face_feature.redMouth(value_1);
    //    cv::imshow("after", res);
    cv::Mat res_copy = res.clone();

    outputImage(res);
}


void MainWindow::showFaceSmooth()
{
    double value_1 = spinBox_1->value();

    double value_2 = spinBox_2->value();

    if (src.empty())
    {
        ui->LabelImage->setText("Please open the image or open the camera!");
        return;
    }

    //    cv::imshow("Filter Vignetting", src);
    FaceFeature face_feature(src);
    res = face_feature.smoothFace(value_1, value_2);
    //    cv::imshow("after", res);
    cv::Mat res_copy = res.clone();

    outputImage(res);
}


void MainWindow::on_pushButton_Open_clicked()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
                                            tr("Open Image"),
                                            ".",
                                            tr("Image Files(*.png *.jpg *.jpeg *.bmp *.JPG)"));

    if(filename.length() <= 0)
    {

        return;
    }

    std::string str = filename.toStdString();
    src = cv::imread(str);
    res = src.clone();
    tmp = src.clone();

    cv::Mat src_copy = src.clone();
    QImage img;
    if (src_copy.channels() == 3)
    {
        cv::Mat hei = src_copy.clone();
        //        cv::imshow("Before", hei);
        cv::cvtColor(src_copy, src_copy, CV_BGR2RGB);
        //        cv::imshow("After", src_copy);

        img = QImage((const uchar*)(src_copy.data), src_copy.cols, src_copy.rows,
                     src_copy.cols * src_copy.channels(), QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const uchar*)(src_copy.data), src_copy.cols, src_copy.rows,
                     src_copy.cols * src_copy.channels(), QImage::Format_Indexed8);
    }

    //    ui->label->setPixmap( QPixmap::fromImage(img));
    //    ui->label->resize( ui->label->pixmap()->size());
    ui->LabelImage->setPixmap( QPixmap::fromImage(img));

    ui->LabelImage->resize( ui->LabelImage->pixmap()->size());

}


void MainWindow::on_pushButton_OpenCamera_clicked()
{
    cv::Mat src;
    QImage img;
    capture.open(0);

    while (1)
    {
        capture >> src;

        if (src.channels() == 3)
        {
            cv::cvtColor(src, src, CV_BGR2RGB);
            img = QImage((const uchar*)(src.data), src.cols, src.rows, src.cols * src.channels(), QImage::Format_RGB888);
        }
        else
        {
            img = QImage((const uchar*)(src.data), src.cols, src.rows, src.cols * src.channels(), QImage::Format_Indexed8);
        }

        ui->LabelImage->setPixmap( QPixmap::fromImage(img));
        ui->LabelImage->resize( ui->LabelImage->pixmap()->size());

        cv::waitKey(30);

    }

}


void MainWindow::on_pushButton_Close_clicked()
{
    //    capture.release();
    close();
}


void MainWindow::on_pushButton_Save_As_clicked()
{
    QString filename;

    filename = QFileDialog::getSaveFileName(this,
                                            tr("Save Image"),
                                            "",
                                            tr("Images (*.png *.bmp *.jpg)")); //选择路径
    std::string fileAsSave = filename.toStdString();
    // TODO
    //    cv::imshow("Save before", res);
    cv::imwrite(fileAsSave, res);

}


void MainWindow::on_pushButton_FilterN_clicked()
{
    res = ImageFilter::FilterNostalgia(src);
    cv::Mat res_copy = res.clone();

    if (src.empty())
    {
        ui->LabelImage->setText("Please open the image or open the camera!");
        return;
    }

    outputImage(res);
}


void MainWindow::on_pushButton_FilterC_clicked()
{
    res = ImageFilter::FilterComics(src);
    cv::Mat res_copy = res.clone();

    if (src.empty())
    {
        ui->LabelImage->setText("Please open the image or open the camera!");
        return;
    }

    outputImage(res);
}


void MainWindow::on_pushButton_FilterE_clicked()
{
    spinBox_1 = new QSpinBox;
    slider_1 = new QSlider(Qt::Horizontal);
    pushButton = new QPushButton;
    small_window = new QWidget;

    small_window->setWindowTitle("Change the Filter Emergence Value");
    spinBox_1->setRange(0, 50);
    slider_1->setRange(0, 50);
    pushButton->setText("Change");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(spinBox_1);
    layout->addWidget(slider_1);
    layout->addWidget(pushButton);

    small_window->setLayout(layout);
    spinBox_1->setValue(5);
    small_window->show();

    QObject::connect(slider_1, SIGNAL(valueChanged(int)), spinBox_1, SLOT(setValue(int)));
    QObject::connect(spinBox_1, SIGNAL(valueChanged(int)), slider_1, SLOT(setValue(int)));
    QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(showFilterE()));

}


void MainWindow::on_pushButton_FilterSa_clicked()
{
    spinBox_1 = new QSpinBox;
    slider_1 = new QSlider(Qt::Horizontal);
    pushButton = new QPushButton;
    small_window = new QWidget;

    small_window->setWindowTitle("Change the Filter Saturation Value");
    spinBox_1->setRange(0, 100);
    slider_1->setRange(0, 100);
    pushButton->setText("Change");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(spinBox_1);
    layout->addWidget(slider_1);
    layout->addWidget(pushButton);

    small_window->setLayout(layout);
    spinBox_1->setValue(30);
    small_window->show();

    QObject::connect(slider_1, SIGNAL(valueChanged(int)), spinBox_1, SLOT(setValue(int)));
    QObject::connect(spinBox_1, SIGNAL(valueChanged(int)), slider_1, SLOT(setValue(int)));
    QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(showFilterSa()));
}


void MainWindow::on_pushButton_FilterG_clicked()
{
    spinBox_1 = new QSpinBox;
    slider_1 = new QSlider(Qt::Horizontal);
    pushButton = new QPushButton;
    small_window = new QWidget;

    small_window->setWindowTitle("Change the Filter Ground Glass Value");
    spinBox_1->setRange(0, 30);
    slider_1->setRange(0, 30);
    pushButton->setText("Change");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(spinBox_1);
    layout->addWidget(slider_1);
    layout->addWidget(pushButton);

    small_window->setLayout(layout);
    spinBox_1->setValue(5);
    small_window->show();

    QObject::connect(slider_1, SIGNAL(valueChanged(int)), spinBox_1, SLOT(setValue(int)));
    QObject::connect(spinBox_1, SIGNAL(valueChanged(int)), slider_1, SLOT(setValue(int)));
    QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(showFilterG()));
}


void MainWindow::on_pushButton_FilterD_clicked()
{
    spinBox_1 = new QSpinBox;
    slider_1 = new QSlider(Qt::Horizontal);
    spinBox_2 = new QSpinBox;
    slider_2 = new QSlider(Qt::Horizontal);
    pushButton = new QPushButton;
    small_window = new QWidget;

    small_window->setWindowTitle("Change the Filter Distinctive Value");
    spinBox_1->setRange(0, 50);
    slider_1->setRange(0, 50);
    spinBox_2->setRange(0, 50);
    slider_2->setRange(0, 50);
    pushButton->setText("Change");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(spinBox_1);
    layout->addWidget(slider_1);
    layout->addWidget(spinBox_2);
    layout->addWidget(slider_2);
    layout->addWidget(pushButton);

    small_window->setLayout(layout);
    spinBox_1->setValue(12);
    spinBox_2->setValue(10);
    small_window->show();

    QObject::connect(slider_1, SIGNAL(valueChanged(int)), spinBox_1, SLOT(setValue(int)));
    QObject::connect(spinBox_1, SIGNAL(valueChanged(int)), slider_1, SLOT(setValue(int)));
    QObject::connect(slider_2, SIGNAL(valueChanged(int)), spinBox_2, SLOT(setValue(int)));
    QObject::connect(spinBox_2, SIGNAL(valueChanged(int)), slider_2, SLOT(setValue(int)));
    QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(showFilterD()));
}


void MainWindow::on_pushButton_FilterSh_clicked()
{
    spinBox_1 = new QSpinBox;
    slider_1 = new QSlider(Qt::Horizontal);
    spinBox_2 = new QSpinBox;
    slider_2 = new QSlider(Qt::Horizontal);
    pushButton = new QPushButton;
    small_window = new QWidget;

    small_window->setWindowTitle("Change the Filter Sharpen Value");
    spinBox_1->setRange(0, 50);
    slider_1->setRange(0, 50);
    spinBox_2->setRange(-20, 20);
    slider_2->setRange(-20, 20);
    pushButton->setText("Change");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(spinBox_1);
    layout->addWidget(slider_1);
    layout->addWidget(spinBox_2);
    layout->addWidget(slider_2);
    layout->addWidget(pushButton);

    small_window->setLayout(layout);
    spinBox_1->setValue(15);
    spinBox_2->setValue(-5);
    small_window->show();

    QObject::connect(slider_1, SIGNAL(valueChanged(int)), spinBox_1, SLOT(setValue(int)));
    QObject::connect(spinBox_1, SIGNAL(valueChanged(int)), slider_1, SLOT(setValue(int)));
    QObject::connect(slider_2, SIGNAL(valueChanged(int)), spinBox_2, SLOT(setValue(int)));
    QObject::connect(spinBox_2, SIGNAL(valueChanged(int)), slider_2, SLOT(setValue(int)));
    QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(showFilterSh()));
}


void MainWindow::on_pushButton_FilterV_clicked()
{
    spinBox_1 = new QSpinBox;
    slider_1 = new QSlider(Qt::Horizontal);
    spinBox_2 = new QSpinBox;
    slider_2 = new QSlider(Qt::Horizontal);
    pushButton = new QPushButton;
    small_window = new QWidget;

    small_window->setWindowTitle("Change the Filter Vignetting Value");
    spinBox_1->setRange(0, 50);
    slider_1->setRange(0, 50);
    spinBox_2->setRange(0, 10);
    slider_2->setRange(0, 10);
    pushButton->setText("Change");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(spinBox_1);
    layout->addWidget(slider_1);
    layout->addWidget(spinBox_2);
    layout->addWidget(slider_2);
    layout->addWidget(pushButton);

    small_window->setLayout(layout);
    spinBox_1->setValue(20);
    spinBox_2->setValue(6);
    small_window->show();

    QObject::connect(slider_1, SIGNAL(valueChanged(int)), spinBox_1, SLOT(setValue(int)));
    QObject::connect(spinBox_1, SIGNAL(valueChanged(int)), slider_1, SLOT(setValue(int)));
    QObject::connect(slider_2, SIGNAL(valueChanged(int)), spinBox_2, SLOT(setValue(int)));
    QObject::connect(spinBox_2, SIGNAL(valueChanged(int)), slider_2, SLOT(setValue(int)));
    QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(showFilterV()));
}


void MainWindow::on_pushButton_FaceBig_clicked()
{
    double value = ui->spinBoxBig->value();

    FaceFeature face_feature(src);

    cv::Mat copy = face_feature.bigEye(value);

    outputImage(copy);
}


void MainWindow::on_pushButton_FaceSmall_clicked()
{
    double value = ui->spinBoxSmall->value();

    FaceFeature face_feature(src);

    cv::Mat copy = face_feature.smallFace(value);

    outputImage(copy);
}


void MainWindow::on_pushButton_MakeUpLight_clicked()
{

    spinBox_1 = new QSpinBox;
    slider_1 = new QSlider(Qt::Horizontal);
    spinBox_2 = new QSpinBox;
    slider_2 = new QSlider(Qt::Horizontal);
    spinBox_3 = new QSpinBox;
    slider_3 = new QSlider(Qt::Horizontal);
    pushButton = new QPushButton;
    small_window = new QWidget;

    small_window->setWindowTitle("Change the Filter Sharpen Value");
    spinBox_1->setRange(0, 10);
    slider_1->setRange(0, 10);
    spinBox_2->setRange(0, 20);
    slider_2->setRange(0, 20);
    spinBox_3->setRange(0, 50);
    slider_3->setRange(0, 50);
    pushButton->setText("Change");

//    QHBoxLayout *layout = new QHBoxLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(spinBox_1);
    layout->addWidget(slider_1);
    layout->addWidget(spinBox_2);
    layout->addWidget(slider_2);
    layout->addWidget(spinBox_3);
    layout->addWidget(slider_3);
    layout->addWidget(pushButton);

    small_window->setLayout(layout);
    spinBox_1->setValue(3);
    spinBox_2->setValue(12);
    spinBox_3->setValue(10);
    small_window->show();

    QObject::connect(slider_1, SIGNAL(valueChanged(int)), spinBox_1, SLOT(setValue(int)));
    QObject::connect(spinBox_1, SIGNAL(valueChanged(int)), slider_1, SLOT(setValue(int)));
    QObject::connect(slider_2, SIGNAL(valueChanged(int)), spinBox_2, SLOT(setValue(int)));
    QObject::connect(spinBox_2, SIGNAL(valueChanged(int)), slider_2, SLOT(setValue(int)));
    QObject::connect(slider_3, SIGNAL(valueChanged(int)), spinBox_3, SLOT(setValue(int)));
    QObject::connect(spinBox_3, SIGNAL(valueChanged(int)), slider_3, SLOT(setValue(int)));
    QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(showFaceLight()));
}


void MainWindow::on_pushButton_MakeUpRed_clicked()
{
    spinBox_1 = new QSpinBox;
    slider_1 = new QSlider(Qt::Horizontal);
    pushButton = new QPushButton;
    small_window = new QWidget;

    small_window->setWindowTitle("Change the Filter Sharpen Value");
    spinBox_1->setRange(0, 50);
    slider_1->setRange(0, 50);
    pushButton->setText("Change");

//    QHBoxLayout *layout = new QHBoxLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(spinBox_1);
    layout->addWidget(slider_1);
    layout->addWidget(pushButton);

    small_window->setLayout(layout);
    spinBox_1->setValue(30);
    small_window->show();

    QObject::connect(slider_1, SIGNAL(valueChanged(int)), spinBox_1, SLOT(setValue(int)));
    QObject::connect(spinBox_1, SIGNAL(valueChanged(int)), slider_1, SLOT(setValue(int)));
    QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(showFaceRed()));
}


void MainWindow::on_pushButton_MakeUpSmooth_clicked()
{

    spinBox_1 = new QSpinBox;
    slider_1 = new QSlider(Qt::Horizontal);
    spinBox_2 = new QSpinBox;
    slider_2 = new QSlider(Qt::Horizontal);
    pushButton = new QPushButton;
    small_window = new QWidget;

    small_window->setWindowTitle("Change the Filter Sharpen Value");
    spinBox_1->setRange(0, 10);
    slider_1->setRange(0, 10);
    spinBox_2->setRange(0, 5);
    slider_2->setRange(0, 5);
    pushButton->setText("Change");

//    QHBoxLayout *layout = new QHBoxLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(spinBox_1);
    layout->addWidget(slider_1);
    layout->addWidget(spinBox_2);
    layout->addWidget(slider_2);
    layout->addWidget(pushButton);

    small_window->setLayout(layout);
    spinBox_1->setValue(3);
    spinBox_2->setValue(1);
    small_window->show();

    QObject::connect(slider_1, SIGNAL(valueChanged(int)), spinBox_1, SLOT(setValue(int)));
    QObject::connect(spinBox_1, SIGNAL(valueChanged(int)), slider_1, SLOT(setValue(int)));
    QObject::connect(slider_2, SIGNAL(valueChanged(int)), spinBox_2, SLOT(setValue(int)));
    QObject::connect(spinBox_2, SIGNAL(valueChanged(int)), slider_2, SLOT(setValue(int)));
    QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(showFaceSmooth()));
}

void MainWindow::on_pushButton_Restore_clicked()
{
    src = tmp;
    outputImage(src);
}
