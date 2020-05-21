QT       += core gui
QT       += multimedia
QT       += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FaceFeature.cpp \
    FaceMakeup.cpp \
    ImageFilter.cpp \
    main.cpp \
    mainwindow.cpp \
    utils.cpp

HEADERS += \
    FaceFeature.h \
    FaceMakeup.h \
    ImageFilter.h \
    mainwindow.h \
    utils.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    CameraOpencv_en_AU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += \
            /opt/ros/kinetic/include/opencv-3.3.1-dev \
            /opt/ros/kinetic/include/opencv-3.3.1-dev/opencv \
            /opt/ros/kinetic/include/opencv-3.3.1-dev/opencv2

INCLUDEPATH += \
            /home/heihei/dlib-19.19

LIBS += /home/heihei/dlib-19.19/examples/build/dlib_build/libdlib.a

LIBS += -L/opt/ros/kinetic/lib/x86_64-linux-gnu \
         -lopencv_stitching3 -lopencv_superres3 -lopencv_videostab3 -lopencv_aruco3 -lopencv_bgsegm3 \
         -lopencv_bioinspired3 -lopencv_ccalib3 -lopencv_cvv3 -lopencv_dpm3 -lopencv_face3 -lopencv_photo3 \
         -lopencv_fuzzy3 -lopencv_hdf3 -lopencv_img_hash3 -lopencv_line_descriptor3 -lopencv_optflow3 -lopencv_reg3 \
         -lopencv_rgbd3 -lopencv_saliency3 -lopencv_stereo3 -lopencv_structured_light3 -lopencv_viz3 -lopencv_phase_unwrapping3 \
         -lopencv_surface_matching3 -lopencv_tracking3 -lopencv_datasets3 -lopencv_text3 -lopencv_dnn3 -lopencv_plot3 \
         -lopencv_xfeatures2d3 -lopencv_shape3 -lopencv_video3 -lopencv_ml3 -lopencv_ximgproc3 -lopencv_calib3d3 -lopencv_features2d3 \
         -lopencv_highgui3 -lopencv_videoio3 -lopencv_flann3 -lopencv_xobjdetect3 -lopencv_imgcodecs3 -lopencv_objdetect3 \
         -lopencv_xphoto3 -lopencv_imgproc3 -lopencv_core3
