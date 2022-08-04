import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 1200
    height: 1000
    title: qsTr("Rücken App")

    StackView{
        id:loader
        anchors.fill: parent
        initialItem: "home"
    }


}
