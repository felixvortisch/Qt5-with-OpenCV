#include "marker.h"

Mat Marker::getRawFrame() const
{
    return rawFrame;
}

void Marker::setRawFrame(const Mat &value)
{
    rawFrame = value;
    convert2Qimage();
}

QImage Marker::getFrame() const
{
    return frame;
}

void Marker::setFrame(const QImage &value)
{
    frame = value;
    Marker::update();
    emit frameChanged();
}

void Marker::openImage(QString url)
{
    url.remove("file://");
    rawFrame = imread(url.toStdString());
    convert2Qimage();
}

void Marker::openMarkerImages(QVariant urls)
{

}

Marker::Marker(QQuickItem *parent): QQuickPaintedItem(parent)
{

}

void Marker::paint(QPainter *painter)
{
    if(!frame.isNull())
    {
        QImage tempFrame = frame.scaled(640, 480,
                         Qt::IgnoreAspectRatio,
                         Qt::FastTransformation);

        painter->drawImage(0, 0, tempFrame, 0, 0,
                           -1, -1, Qt::AutoColor);
    }
}

void Marker::convert2Qimage()
{
    Mat tempMat;
    cvtColor(rawFrame, tempMat, COLOR_RGB2BGR);
    QImage tempImage((uchar*) tempMat.data, tempMat.cols, tempMat.rows, tempMat.step, QImage::Format_RGB888);

    frame = tempImage;
    frame.detach();
    Marker::update();
    emit (frameChanged());
}



