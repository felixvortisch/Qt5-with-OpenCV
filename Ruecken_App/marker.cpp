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

void Marker::solve_PnP()
{
    std::vector<cv::Point3f> objectPoints;
    std::vector<cv::Point2f> imagePoints;
    cv::Mat rvecs, tvecs;

    objectPoints.push_back(cv::Point3f(0.0, 0.0, 0.0));
    objectPoints.push_back(cv::Point3f(0.1475, 0.234, 0.0));
    objectPoints.push_back(cv::Point3f(0.1475, 0.0, 0.0));
    objectPoints.push_back(cv::Point3f(0.0, 0.234, 0.0));

    qDebug() << m_markerCorners.at(0).at(0).x << " " << m_markerCorners.at(0).at(0).y;

    imagePoints.push_back(m_markerCorners.at(3).at(0));
    imagePoints.push_back(m_markerCorners.at(2).at(0));
    imagePoints.push_back(m_markerCorners.at(4).at(0));
    imagePoints.push_back(m_markerCorners.at(1).at(0));

    cv::solvePnP(objectPoints, imagePoints, m_K, m_k, rvecs, tvecs);



    cv::FileStorage fs("PnP.xml", cv::FileStorage::WRITE);

    fs << "tvecs" << tvecs;
    fs << "rvecs" << rvecs;



    fs.release();
}

void Marker::openImage(QString url)
{
    url.remove("file://");
    rawFrame = imread(url.toStdString());
    convert2Qimage();
}

void Marker::openMarkerImages(QVariant urls)
{
    QList<QUrl> strList = QVariant(urls).value<QList<QUrl> >();

    for(int i=0; i < strList.size(); i++)
    {
        QString temp = strList.at(i).toString();
        temp.remove("file://");
        cv::String tempStdString = temp.toStdString();
        rawFrames.append(imread(tempStdString));
        fileNames.push_back(tempStdString);
        qDebug() << fileNames.size();
    }

    convert2Qimage(rawFrames.at(bildIndex));
}

void Marker::generateMarkers()
{
    for(int i = 0; i < 5; i++)
    {
        QString s = "marker" + QString::number(i) +".png";
        cv::Mat markerImage;
        cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
        cv::aruco::drawMarker(dictionary, i, 200, markerImage, 1);
        cv::imwrite(s.toStdString(), markerImage);
    }

}

void Marker::detectMarkers()
{
    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
    cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::aruco::detectMarkers(rawFrames.at(bildIndex), dictionary, markerCorners, markerIds, parameters, rejectedCandidates);

    cv::Mat outputImage = rawFrames.at(bildIndex).clone();
    cv::aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);

    m_markerCorners = markerCorners;
    m_markerIds = markerIds;
    cv::FileStorage fs("markerCorners.xml", cv::FileStorage::WRITE);

    fs << "marker_Corners" << m_markerCorners;
    fs << "markerIds" << markerIds;



    fs.release();


    convert2Qimage(outputImage);
}

void Marker::loadCalibrationParameters()
{
    cv::FileStorage fs("camera.xml", cv::FileStorage::READ);

    fs["image_width"] >> frameSize.width;
    fs["image_height"] >> frameSize.height;
    fs["camera_matrix"] >> m_K;
    fs["distortion_coefficients"] >> m_k;


}

void Marker::indexPlus()
{
    if(bildIndex < rawFrames.size()-1)
    {
        bildIndex += 1;
    }else{
        bildIndex=0;
    }
    convert2Qimage(rawFrames.at(bildIndex));
}

void Marker::indexMinus()
{
    if(bildIndex > 0 )
    {
        bildIndex -= 1;
    }else{
        bildIndex=rawFrames.size()-1;
    }
    convert2Qimage(rawFrames.at(bildIndex));
}

void Marker::poseEstimation()
{
    Mat outputImage;
    std::vector<cv::Vec3d> rvecs, tvecs;
    cv::aruco::estimatePoseSingleMarkers(m_markerCorners, 0.0428f, m_K, m_k, rvecs, tvecs);

    rawFrames.at(bildIndex).copyTo(outputImage);
    for (int i = 0; i < rvecs.size(); ++i) {
        auto rvec = rvecs[i];
        auto tvec = tvecs[i];
        cv::drawFrameAxes(outputImage, m_K, m_k, rvec, tvec, 0.3);
    }

    cv::FileStorage fs("markerCorners.xml", cv::FileStorage::WRITE);

    fs << "marker_Corners" << m_markerCorners;
    fs << "rotation_vector" << rvecs;
    fs << "translation_vector" << tvecs;

    fs.release();
    convert2Qimage(outputImage);
}

void Marker::estimateCameraPosition()
{
    solve_PnP();
}

Marker::Marker(QQuickItem *parent): QQuickPaintedItem(parent)
{
    bildIndex = 0;
}

void Marker::paint(QPainter *painter)
{
    if(!frame.isNull())
    {
        QImage tempFrame = frame.scaled(800, 600,
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

void Marker::convert2Qimage(Mat raw)
{
    Mat tempMat;
    cvtColor(raw, tempMat, COLOR_RGB2BGR);
    QImage tempImage((uchar*) tempMat.data, tempMat.cols, tempMat.rows, tempMat.step, QImage::Format_RGB888);

    frame = tempImage;
    frame.detach();
    Marker::update();
    emit (frameChanged());
}


