import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

Item {
    id: item1
    width: 720
    height: 1200
    Rectangle {
        id: rectangle
        width: 720
        height: 1280
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#08104d"
            }

            GradientStop {
                position: 1
                color: "#000000"
            }
        }
        anchors.rightMargin: 0
        anchors.fill: parent


        RowLayout {
            id: statusBarRowLayout
            height: 48
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            spacing: 30

            RowLayout {
                id: rowLayout2
                width: 100
                height: 100
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: false
                spacing: 0

                Text {
                    id: satellitesCount
                    color: "#ffffff"
                    text: qsTr("0")
                    topPadding: 27
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: 20
                }

                Image {
                    id: satellitesImage
                    width: 48
                    height: 48
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    source: "images/satellite.png"

                    SequentialAnimation {
                        id: sateliteAnimation
                        running: true
                        loops: Animation.Infinite
                        OpacityAnimator {
                            target: satellitesImage
                            from: 0;
                            to: 1;
                            duration: 1000
                        }

                        OpacityAnimator {
                            target: satellitesImage
                            from: 1;
                            to: 0;
                            duration: 1000
                        }

                    }


                }
            }

            Image {
                id: registrationStatusImage
                width: 48
                height: 48
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                source: "images/stop_rec.png"
            }

            Image {
                id: registrationOptionImage
                width: 48
                height: 48
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                source: "images/increase.png"
            }

            Image {
                id: connectionStatusImage
                width: 48
                height: 48
                Layout.rowSpan: 1
                Layout.fillHeight: false
                Layout.fillWidth: false
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                source: "images/disconnected.png"
            }


        }

        Rectangle {
            id: nextValueBackground
            x: 160
            y: 522
            width: item1.height / 3
            height: width
            color: "#ffffff"
            radius: height / 2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: plusMinusSetColumnLayout.top
            anchors.bottomMargin: 50

            Label {
                id: nextValue
                anchors.centerIn: parent
                width: nextValue.height
                color: "#e90000"
                text: qsTr("6000/6001 км")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: nextValueBackground.height/8
                font.bold: true
            }

        }

        Button {
            id: marksButton
            y: 133
            height: item1.height/10
            text: "Отметки"
            anchors.bottom: columnLayout.top
            anchors.bottomMargin: 70
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
            font.pointSize: 30
            Layout.minimumWidth: 220
            Layout.fillHeight: true
            scale: 1
            focusPolicy: Qt.NoFocus

            contentItem: Text {
                text: marksButton.text
                font.pointSize: 24
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: marksButton.down ? "black" : "white"
            }

            background: Rectangle {
                border.color: "white"
                border.width: 2
                color: marksButton.down ? "white" : "#00000000"
                radius: 5
            }
            onReleased: {
                view.currentIndex = 2
            }
        }

        ColumnLayout {
            id: plusMinusSetColumnLayout
            y: 942
            height: item1.height / 5
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            spacing: 20

            RowLayout {
                id: rowLayout1
                width: 620
                height: 76
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 20

                Button {
                    id: minusButton
                    text: " -"
                    Layout.fillWidth: true
                    focusPolicy: Qt.NoFocus
                    Layout.fillHeight: true
                    font.pixelSize: 30
                    contentItem: Text {
                        font.pointSize: 0
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        color: minusButton.down ? "black" : "white"
                        text: "-"
                    }

                    background: Rectangle {
                        border.color: "white"
                        border.width: 2
                        color: minusButton.down ? "white" : "#00000000"
                        radius: 5
                        anchors.fill:parent
                    }
                    onClicked: {
                        backend.prevTrackmark()
                    }

                }

                Button {
                    id: plusButton
                    text: "+"
                    font.pixelSize: 30
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    focusPolicy: Qt.NoFocus

                    contentItem: Text {
                        text: plusButton.text
                        textFormat: Text.PlainText
                        font.pointSize: 0
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        color: plusButton.down ? "black" : "white"
                    }

                    background: Rectangle {
                        border.color: "white"
                        border.width: 2
                        color: plusButton.down ? "white" : "#00000000"
                        radius: 5
                        anchors.fill:parent
                    }
                    onClicked: {
                        backend.nextTrackmark()
                    }
                }
            }

            Button {
                id: setButton
                text: qsTr("Установить")
                font.pointSize: 30
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
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
                    color: setButton.down ? "white" : "#00000000"
                    radius: 5
                    anchors.fill:parent
                }

                onReleased: {
                    backend.setTrackMarks()
                }
            }
        }

        ColumnLayout {
            id: columnLayout
            y: 311
            width: 151
            height: 129
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: nextValueBackground.top
            anchors.bottomMargin: 50

            RowLayout {
                id: rowLayout
                width: 232
                height: 45
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                spacing: 10
                Layout.fillHeight: true
                Layout.fillWidth: true

                Text {
                    id: speedValue
                    color: "#ffffff"
                    text: qsTr("0")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillHeight: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: false
                    font.pixelSize: item1.height / 25
                }

                Text {
                    id: speedLabel
                    color: "#ffffff"
                    text: qsTr("км/ч")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: false
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: item1.height / 25
                }
            }

            Text {
                id: currentCoordinate
                width: 167
                height: 48
                color: "#ffffff"
                text: "0 km"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                font.pixelSize: item1.height / 20
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
            currentCoordinate.text = value
        }
        onDoNextTrackMarks: {
            nextValue.text = value
        }
        onSatellitesFound: {
            sateliteAnimation.stop()
            satellitesImage.opacity = 1
        }
        onSatellitesNotFound: {
            sateliteAnimation.start()
        }
        onSatellitesCount: {
            satellitesCount.text = count
        }
    }
}


