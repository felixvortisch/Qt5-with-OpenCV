
import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import com.app.frame 1.0
import com.app.marker 1.0

Page {
    width: parent.width
    height: parent.height

    Rectangle{
        id: bar
        width: parent.width
        height: parent.height * 0.1
        color: "orange"

        Label{
            text: "Marker Detection"
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


        Marker{
            id: marker
            Layout.preferredWidth: imageContainer.width
            Layout.preferredHeight: imageContainer.height
            visible: false
        }

    }

    RowLayout{
        id: firstRow
        anchors.top: imageContainer.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width*0.6
        spacing: width*0.2

        Button{
            id: zBtn
            text: "Back"
            Layout.preferredWidth: parent.width*0.3
            visible: false
            onClicked: marker.indexMinus()
        }

        Button{
            id:wBtn
            text: "Next"
            Layout.preferredWidth: parent.width*0.3
            visible: false
            onClicked: marker.indexPlus()
        }

    }


    RowLayout{
        id: secondRow
        anchors.top: firstRow.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width*0.6
        spacing: width*0.2

        Button{
            text: "Open Images for Marker detection"
            Layout.preferredWidth: parent.width*0.2
            onClicked: markerImageDialog.open()
        }

        Button{
            text: "Generate Markers"
            Layout.preferredWidth: parent.width*0.2
            onClicked: marker.generateMarkers()
        }

        Button{
            text: "Go Back"
            Layout.preferredWidth: parent.width*0.2
            onClicked: loader.pop()
        }
    }

    RowLayout{
        id: thirdRow
        anchors.top: secondRow.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width*0.6
        spacing: width*0.2

        Button{
            text: "Load Calibration Parameters"
            Layout.preferredWidth: parent.width*0.2
            onClicked: marker.loadCalibrationParameters()
        }

        Button{
            text: "Detect Markers"
            Layout.preferredWidth: parent.width*0.2
            onClicked: marker.detectMarkers()
        }

        Button{
            text: "Pose Estimation"
            Layout.preferredWidth: parent.width*0.2
            onClicked: marker.poseEstimation()
        }

    }

    RowLayout{
        anchors.top: thirdRow.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width*0.6
        spacing: width*0.2

        Button{
            text: "Estimate Camera Position"
            Layout.preferredWidth: parent.width*0.2
            onClicked: marker.estimateCameraPosition()
        }



    }

    FileDialog{
        id: markerImageDialog
        title: "Choose Images"
        folder: shortcuts.pictures
        selectMultiple: true

        onAccepted: {
            marker.visible = true;
            marker.openMarkerImages(markerImageDialog.fileUrls);
            zBtn.visible = true
            wBtn.visible = true
        }
    }

}
