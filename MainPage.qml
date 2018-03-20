import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

Item {
    id: item1
    width: 480
    height: 800
    ColumnLayout {
        anchors.rightMargin: 15
        anchors.leftMargin: 15
        anchors.bottomMargin: 30
        anchors.topMargin: 30
        anchors.fill: parent
        RowLayout {
            id: statusBarRowLayout
            x: 162
            width: 156
            height: 31
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 30

            Image {
                id: registrationStatusImage
                width: 48
                height: 48
                source: "images/stop_rec.png"
            }

            Image {
                id: registrationOptionImage
                width: 48
                height: 48
                source: "images/increase.png"
            }

            Image {
                id: connectionStatusImage
                width: 48
                height: 48
                source: "images/disconnected.png"
            }
        }

        RowLayout {
            id: rowLayout
            x: 134
            y: 194
            width: 232
            height: 45
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                id: speedValue
                color: "#ffffff"
                text: qsTr("0")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: false
                font.pixelSize: 30
            }

            Text {
                id: speedLabel
                color: "#ffffff"
                text: qsTr("км/ч")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 30
            }
        }

        RowLayout {
            id: currentCoordinateLayout
            x: 94
            width: 466
            height: 98
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

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
            width: 300
            height: 300
            radius: height/2
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: currentCoordinateLayout.horizontalCenter

            Label {
                id: nextValue
                anchors.centerIn: parent
                width: nextValue.height
                color: "#1d325e"
                text: qsTr("6/7 пк")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 50
                font.bold: true
            }

        }

        ColumnLayout {
            Layout.fillHeight: false
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20

            RowLayout {
                id: plusMinusRowLayout
                Layout.fillWidth: true
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter

                CustomButton {
                    id: minusButton
                    Layout.minimumWidth: 220
                    height: window.height / 10
                    text: qsTr("-")
                    focusPolicy: Qt.NoFocus
                    Layout.fillHeight: true
                    onClicked: {
                        backend.prevTrackmark()
                    }
                }

                CustomButton {
                    id: plusButton
                    Layout.minimumWidth: 220
                    height: window.height / 10
                    text: qsTr("+")
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.fillHeight: true
                    focusPolicy: Qt.NoFocus
                    onClicked: {
                        backend.nextTrackmark()
                    }
                }
            }

            CustomButton {
                id: setButton
                Layout.minimumWidth: plusMinusRowLayout.width
                text: qsTr("Установить")
                Layout.fillHeight: false
                Layout.fillWidth: false
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
                onReleased: {
                    backend.setTrackMarks()
                }
            }
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

        onDoCurrentMeter: {
            mValue.text = m
        }

        onDoStartRegistration: {
            registrationStatusImage.source = "images/start_rec.png"
        }

        onDoStopRegistration: {
            registrationStatusImage.source = "images/stop_rec.png"
        }

        onDoIncrease: {
            registrationOptionImage.source = "images/increase.png"
        }

        onDoDecrease: {
            registrationOptionImage.source = "images/decrease.png"
        }
        onDoCurrentSpeed: {
            (speed < 10) ? speedValue.text = Number(speed).toPrecision(2)
                         : speedValue.text = Number(speed).toPrecision(3)
        }
        onDoCurrentTrackMarks: {
            kmValue.text = km
            pkValue.text = pk
            mValue.text = m
        }
        onDoNextTrackMarks: {
            nextValue.text = value
        }
    }
}


