
import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import com.app.frame 1.0
import com.app.calibrate 1.0

Page {
    width: parent.width
    height: parent.height

    Rectangle{
        id: bar
        width: parent.width
        height: parent.height * 0.1
        color: "orange"

        Label{
            text: "Calibrate Camera"
            color: "white"
            anchors.centerIn: parent
        }
    }

    ColumnLayout
    {
        id: imageContainer
        width: 800
        height: 600
        anchors.top: bar.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter


        Calibrate{
            id: selectedImages
            Layout.preferredWidth: imageContainer.width
            Layout.preferredHeight: imageContainer.height
            visible: false
        }

    }

    RowLayout{
        anchors.top: imageContainer.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width*0.6
        spacing: width*0.2

        Button{
            text: "Open Images for Calibration"
            Layout.preferredWidth: parent.width*0.4
            onClicked: imageDialog.open()
        }

        Button{
            text: "Safe Calibration Parameters"
            Layout.preferredWidth: parent.width*0.4
            onClicked: selectedImages.saveCalibrationParameters()
        }

        Button{
            text: "Go Back"
            Layout.preferredWidth: parent.width*0.4
            onClicked: loader.pop()
        }
    }

    FileDialog{
        id: imageDialog
        title: "Choose Images"
        folder: shortcuts.pictures
        selectMultiple: true

        onAccepted: {
            selectedImages.visible = true;
            selectedImages.openCalibrationImages(imageDialog.fileUrls);
        }
    }

}
