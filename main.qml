import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import remote_controller.appcore 1.0
import Qt.labs.calendar 1.0

Window {
    id: window
    visible: true
    width: 480
    height: 800
    color: "#1d325e"
    opacity: 1

    AppCore {
        id: backend
    }

    Drawer {
        id: drawer
        width: window.width * 0.66
        height: window.height
        background: Rectangle {
            color: "white"
        }

        Column {
            id: column
            anchors.fill: parent

            ItemDelegate {
                id: connectItem
                text: qsTr("Подключиться")
                anchors.topMargin: 10
                width: parent.width
                onClicked: {
                    drawer.close()
                    backend.onConnectingToServer()
                }
            }
            ItemDelegate {
                id: disconnectItem
                text: qsTr("Отключиться")
                anchors.topMargin: 50
                width: parent.width
                onClicked: {
                    drawer.close()
                    backend.onDisconnectingToServer()
                }
            }

            ItemDelegate {
                id: exitItem
                text: qsTr("Выход")
                anchors.topMargin: 130
                width: parent.width
                onClicked: {
                    window.close()
                }
            }
        }
    }

    RowLayout {
        id: currentCoordinateLayout
        x: 94
        width: 466
        height: 98
        anchors.horizontalCenterOffset: 0
        anchors.top: statusBarRowLayout.bottom
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 0

        Text {
            id: kmValue
            width: 44
            height: 39
            color: "#ffffff"
            text: qsTr("-")
            font.pixelSize: 50
        }

        Text {
            id: kmLabel
            width: 44
            height: 39
            color: "#ffffff"
            text: qsTr("км")
            font.pixelSize: 40
        }

        Text {
            id: pkValue
            width: 44
            height: 39
            color: "#ffffff"
            text: qsTr("-")
            font.pixelSize: 50
        }


        Text {
            id: pkLabel
            width: 44
            height: 39
            color: "#ffffff"
            text: qsTr("пк")
            font.pixelSize: 40
        }

        Text {
            id: mValue
            width: 44
            height: 39
            color: "#ffffff"
            text: qsTr("-")
            font.pixelSize: 50
        }

        Text {
            id: mLabel
            width: 44
            height: 39
            color: "#ffffff"
            text: qsTr("м")
            font.pixelSize: 40
        }
    }

    Rectangle {
        id: nextValueBackground
        x: 170
        width: 250
        height: 250
        radius: height/2
        anchors.horizontalCenterOffset: 0
        anchors.top: currentCoordinateLayout.bottom
        anchors.topMargin: 30
        anchors.horizontalCenter: currentCoordinateLayout.horizontalCenter

        Label {
            id: nextValue
            anchors.centerIn: parent
            width: nextValue.height
            color: "#1d325e"
            text: qsTr("6 ПК")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 60
            font.bold: true
        }
    }

    Row {
        id: plusMinusRowLayout
        width: 466
        height: 86
        spacing: 20
        anchors.horizontalCenterOffset: 0
        anchors.top: nextValueBackground.bottom
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter

        Button {
            id: minusButton
            width: 223
            height: 80
            text: qsTr("-")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            focusPolicy: Qt.NoFocus
            Layout.fillHeight: true
            Layout.fillWidth: true
            font.pointSize: 24

            contentItem: Text {
                text: minusButton.text
                font.pointSize: minusButton.font.pointSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: minusButton.down ? "black" : "white"
                font.bold: true
            }

            background: Rectangle {
                border.color: "white"
                border.width: 2
                color: minusButton.down ? "white" : "#1d325e"
                radius: 5
            }
        }

        Button {
            id: plusButton
            width: 223
            height: 80
            text: qsTr("+")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            focusPolicy: Qt.NoFocus
            font.pointSize: 24

            contentItem: Text {
                text: plusButton.text
                font.pointSize: minusButton.font.pointSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: plusButton.down ? "black" : "white"
                font.bold: true
            }

            background: Rectangle {
                border.color: "white"
                border.width: 2
                color: plusButton.down ? "white" : "#1d325e"
                radius: 5
            }

            onClicked: {

            }
        }
    }
    Button {
        id: setButton
        x: 135
        width: 467
        height: 80
        text: qsTr("Установить")
        anchors.horizontalCenterOffset: 1
        anchors.top: plusMinusRowLayout.bottom
        anchors.topMargin: 70
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 24

        contentItem: Text {
            text: setButton.text
            font.pointSize: 24
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: setButton.down ? "black" : "white"
        }

        background: Rectangle {
            border.color: "white"
            border.width: 2
            color: setButton.down ? "white" : "#1d325e"
            radius: 5
        }

        onReleased: {
            backend.checkDistance()
        }
    }

    RowLayout {
        id: statusBarRowLayout
        x: 162
        width: 156
        height: 31
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 19
        spacing: 30

        Image {
            id: registrationStatusImage
            width: 28
            height: 28
            Layout.fillHeight: true
            Layout.fillWidth: true
            source: "images/stop_rec.png"
        }

        Image {
            id: registrationOptionImage
            width: 28
            height: 28
            Layout.fillWidth: true
            Layout.fillHeight: true
            source: "images/increase.png"
        }

        Image {
            id: connectionStatusImage
            width: 28
            height: 28
            Layout.fillWidth: true
            Layout.fillHeight: true
            source: "images/disconnected.png"
        }
    }

    Connections {
        target: backend
        onDoSocketConnected: {
            connectionStatusImage.source = "images/connected.png"
        }

        onDoSocketDisconnected: {
            connectionStatusImage.source = "images/disconnected.png"
        }

        onNewData: {
            kmValue.text = km
            pkValue.text = pk
            mValue.text = m
        }
    }
}
