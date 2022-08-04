#ifndef CALIBRATE_H
#define CALIBRATE_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>
#include <QDebug>


#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>


using namespace cv;

class Calibrate : public QQuickPaintedItem
{Q_OBJECT
    Q_PROPERTY(QImage frame READ getFrame WRITE setFrame NOTIFY frameChanged)
    Q_PROPERTY(QImage calibratedImage READ getCalibratedImage WRITE setCalibratedImage NOTIFY calibratedImageChanged)
private:
    Mat rawFrame;
    QList<Mat> rawCalibFrames;
    QList<Mat> rawCalibImages;
    QList<Mat> calibFrames;
    std::vector<cv::String> fileNames;
    QImage frame;
    QImage calibratedImage;

    std::vector<std::vector<cv::Point2f>> m_q;
    std::vector<std::vector<cv::Point3f>> m_Q;
    cv::Size frameSize;
    cv::Matx33f m_K; //instinsic
    cv::Vec<float, 5> m_k; // distortion

public:
    Calibrate(QQuickItem *parent = 0);
    void paint(QPainter *painter);
    Mat getRawFrame() const;
    Mat getCalibRawFrame() const;
    Q_INVOKABLE void setRawFrame(const Mat &value);
    Q_INVOKABLE void setCalibRawFrame(const Mat &value);
    QImage getFrame() const;
    QImage getCalibratedImage() const;
    void setFrame(const QImage &value);
    void setCalibratedImage(const QImage &value);
    void calculateCalibParameters();
    void calibrateImage();
    Q_INVOKABLE void openImage(QString url);
    Q_INVOKABLE void openCalibrationImages(QVariant urls);

signals:
    void frameChanged();
    void calibratedImageChanged();

public slots:
    void convert2Qimage();

};

#endif // CALIBRATE_H
