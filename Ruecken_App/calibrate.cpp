#include "calibrate.h"

Mat Calibrate::getRawFrame() const
{
    return rawFrame;
}

Mat Calibrate::getCalibRawFrame() const
{
    return rawCalibImages.at(0);
}

void Calibrate::setRawFrame(const Mat &value)
{
    rawFrame = value;
    convert2Qimage();
}

void Calibrate::setCalibRawFrame(const Mat &value)
{
    rawCalibImages.insert(0, value);
    convert2Qimage();
}

QImage Calibrate::getFrame() const
{
    return frame;
}

QImage Calibrate::getCalibratedImage() const
{
    return calibratedImage;
}

void Calibrate::setFrame(const QImage &value)
{
    frame = value;
    Calibrate::update();
    emit frameChanged();
}

void Calibrate::setCalibratedImage(const QImage &value)
{
    calibratedImage = value;
    Calibrate::update();
    emit calibratedImageChanged();
}

void Calibrate::calculateCalibParameters()
{
    cv::Size patternSize(13 - 1, 9 - 1);
    std::vector<std::vector<cv::Point2f>> q(fileNames.size());


    std::vector<std::vector<cv::Point3f>> Q;


    int checkerBoard[2] = {13,9};
    float fieldSize = 23.8f;

    std::vector<cv::Point3f> objp;

    for(int i = 1; i < checkerBoard[1]; i++)
    {
        for(int j = 1; j < checkerBoard[0]; j++)
        {
            objp.push_back(cv::Point3f(j*fieldSize, i*fieldSize, 0));
        }
    }

    std::vector<cv::Point2f> imgPoint;

    std::size_t i = 0;
    for (auto const &f : fileNames)
    {
        cv::Mat img = cv::imread((fileNames[i]));
        if(i==0 && !img.empty())
        {
            frameSize = img.size();
            qDebug() << frameSize.height << " " << frameSize.width;
        }
        qDebug() << f.c_str() << "  " << fileNames[i].c_str();
        cv::Mat gray;

        cv::cvtColor(img, gray, cv::COLOR_RGB2GRAY);

        bool patternFound = cv::findChessboardCorners(gray, patternSize, q[i], cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

        if(patternFound)
        {
            cv::cornerSubPix(gray, q[i], cv::Size(11,11), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER,30, 0.1));
            Q.push_back(objp);

        }

        cv::drawChessboardCorners(img, patternSize, q[i], patternFound);
        m_q = q;
        m_Q = Q;
        calibFrames.append(img);
        //cv::imshow("cd", img);
        //setRawFrame(img);
        //cv::waitKey(0);

        i++;
    }
    setRawFrame(calibFrames.at(0));
    calibrateImage();
}

void Calibrate::calibrateImage()
{
    cv::Matx33f K(cv::Matx33f::eye());
    cv::Vec<float, 5> k(0,0,0,0,0);
    std::vector<cv::Mat> rvecs, tvecs;
    std::vector<double> stdIntrinsics, stdExtrinsics, perViewErrors;
    int flags = cv::CALIB_FIX_ASPECT_RATIO + cv::CALIB_FIX_K3 +
                cv::CALIB_ZERO_TANGENT_DIST + cv::CALIB_FIX_PRINCIPAL_POINT;

    double error = cv::calibrateCamera(m_Q, m_q, frameSize, K, k, rvecs, tvecs, flags);
    qDebug() << "Reprojection error: " << error;

    cv::Mat mapX, mapY;
    cv::initUndistortRectifyMap(K, k, cv::Matx33f::eye(), K, frameSize, CV_32FC1,
                                mapX, mapY);

    for(auto const &f : fileNames)
    {
        cv::Mat img = cv::imread(f, cv::IMREAD_COLOR);

        cv::Mat imgUndistorted;

        cv::remap(img, imgUndistorted, mapX, mapY, cv::INTER_LINEAR);

        rawCalibImages.append(imgUndistorted);
    }
    m_K = K;
    m_k = k;
    setCalibRawFrame(rawCalibImages.at(0));
}

void Calibrate::openImage(QString url)
{
    url.remove("file://");
    rawFrame = imread(url.toStdString());
    convert2Qimage();
}

void Calibrate::openCalibrationImages(QVariant urls)
{
    QList<QUrl> strList = QVariant(urls).value<QList<QUrl> >();

    for(int i=0; i < strList.size(); i++)
    {
        QString temp = strList.at(i).toString();
        temp.remove("file://");
        cv::String tempStdString = temp.toStdString();
        rawCalibFrames.append(imread(tempStdString));
        fileNames.push_back(tempStdString);
        qDebug() << fileNames.size();
    }
    calculateCalibParameters();
}



void Calibrate::saveCalibrationParameters()
{
    cv::FileStorage fs("camera.xml", cv::FileStorage::WRITE);

    fs << "image_width" << frameSize.width;
    fs << "image_height" << frameSize.height;
    fs << "camera_matrix" << m_K;
    fs << "distortion_coefficients" << m_k;

    fs.release();



}


Calibrate::Calibrate(QQuickItem *parent): QQuickPaintedItem(parent)
{

}

void Calibrate::paint(QPainter *painter)
{
    if(!frame.isNull())
    {
        QImage tempFrame = frame.scaled(400, 300,
                         Qt::IgnoreAspectRatio,
                         Qt::FastTransformation);

        painter->drawImage(0, 0, tempFrame, 0, 0,
                           -1, -1, Qt::AutoColor);
    }

    if(!calibratedImage.isNull())
    {
        QImage tempFrame = calibratedImage.scaled(400, 300,
                         Qt::IgnoreAspectRatio,
                         Qt::FastTransformation);

        painter->drawImage(410, 0, tempFrame, 0, 0,
                           -1, -1, Qt::AutoColor);
    }
}

void Calibrate::convert2Qimage()
{
    Mat tempMat;
    if(!rawFrame.empty())
    {
        cvtColor(rawFrame, tempMat, COLOR_RGB2BGR);
        QImage tempImage((uchar*) tempMat.data, tempMat.cols, tempMat.rows, tempMat.step, QImage::Format_RGB888);

        frame = tempImage;
        frame.detach();
        Calibrate::update();
        emit (frameChanged());
    }
    if(!rawCalibImages.empty())
    {
        cvtColor(rawCalibImages.at(0), tempMat, COLOR_RGB2BGR);
        QImage tempImage((uchar*) tempMat.data, tempMat.cols, tempMat.rows, tempMat.step, QImage::Format_RGB888);

        calibratedImage = tempImage;
        calibratedImage.detach();
        Calibrate::update();
        emit (calibratedImageChanged());
    }
}
