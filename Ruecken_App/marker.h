#ifndef MARKER_H
#define MARKER_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

class Marker : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage frame READ getFrame WRITE setFrame NOTIFY frameChanged)
private:
    Mat rawFrame;
    QImage frame;
    QList<Mat> rawFrames;
    std::vector<cv::String> fileNames;

public:
    Marker(QQuickItem *parent = 0);
    void paint(QPainter *painter);
    Mat getRawFrame() const;
    Q_INVOKABLE void setRawFrame(const Mat &value);
    QImage getFrame() const;
    void setFrame(const QImage &value);
    Q_INVOKABLE void openImage(QString url);
    Q_INVOKABLE void openMarkerImages(QVariant urls);

signals:
    void frameChanged();

public slots:
    void convert2Qimage();

};


#endif // MARKER_H
