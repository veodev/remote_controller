import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

Page {
    id: page
//    width: 720
//    height: 1280
//    anchors.fill: parent
    title: qsTr("Главная")

    function stopBlinking() {
        nextValueAnimation.stop()        
        nextValue.opacity = 1
        nextValueBackground.border.width = 0
        setButtonTextAnimation.stop()
        setButton.opacity = 1
    }

    Rectangle {
        id: rectangle
        width: 720
        height: 1280
        anchors.rightMargin: 0
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {position: 0; color: "#08104d"}
            GradientStop {position: 1; color: "#000000"}
        }

        RowLayout {
            id: statusBarRowLayout
            height: 85
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            spacing: 30

            RowLayout {
                id: rowLayout2
                width: 100
                height: 100
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: false
                spacing: 10

                Text {
                    id: satellitesCount
                    width: 48
                    height: 4
                    color: "#ffffff"
                    text: qsTr("0")
                    font.family: "Times New Roman"
                    Layout.minimumHeight: 48
                    Layout.minimumWidth: 30
                    topPadding: 50
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: 30
                }

                Image {
                    id: satellitesImage
                    width: 48
                    height: 48
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    source: "images/satellites_notfound_72.png"

                    SequentialAnimation {
                        id: sateliteAnimation
                        running: true
                        loops: Animation.Infinite
                        OpacityAnimator {target: satellitesImage; from: 0; to: 1; duration: 500}
                        OpacityAnimator {target: satellitesImage; from: 1; to: 0; duration: 500}
                    }
                }
            }

            Image {
                id: registrationStatusImage
                width: 48
                height: 48
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                source: "images/stop_rec_72.png"

                SequentialAnimation {
                    id: registrationAnimation
                    running: false
                    loops: Animation.Infinite
                    OpacityAnimator {target: registrationStatusImage; from: 0; to: 1; duration: 1000}
                    OpacityAnimator {target: registrationStatusImage; from: 1; to: 0; duration: 1000}
                }
            }

            Image {
                id: registrationOptionImage
                width: 48
                height: 48
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                source: "images/increase_72.png"
            }

            Image {
                id: connectionStatusImage
                width: 48
                height: 48
                Layout.rowSpan: 1
                Layout.fillHeight: false
                Layout.fillWidth: false
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                source: "images/disconnected_72.png"

                SequentialAnimation {
                    id: connectionAnimation
                    running: true
                    loops: Animation.Infinite
                    OpacityAnimator {target: connectionStatusImage; from: 0; to: 1; duration: 500}
                    OpacityAnimator {target: connectionStatusImage; from: 1; to: 0; duration: 500}
                }
            }
        }

        Rectangle {
            id: nextValueBackground
            x: 160
            y: 734            
            width: page.width - 40
            height: width / 2
            color: "#ffffff"
            radius: height / 10
            anchors.bottom: minusButton.top
            anchors.bottomMargin: 65
            anchors.horizontalCenterOffset: 1
            anchors.horizontalCenter: parent.horizontalCenter
            border.color: "red"
            border.width: 0

            Label {
                id: nextValue                
                anchors.centerIn: parent
                width: nextValue.height
                color: "#08104d"
                text: qsTr("00000/00000 км")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: nextValueBackground.height/5
                font.bold: true

                SequentialAnimation {
                    id: nextValueAnimation
                    running: false
                    loops: Animation.Infinite
                    OpacityAnimator {target: nextValue; from: 0.3; to: 1; duration: 500}
                    OpacityAnimator {target: nextValue; from: 1; to: 0.3; duration: 500}
                }               
            }
            Button {
                anchors.fill: parent
                background: Rectangle {
                    border.color: "#00000000"
                    color: "#00000000"
                }
                onReleased: {
                    backend.setTrackMark()
                    stopBlinking()
                }
            }
        }

        CustomButton {            
            id: marksButton            
            height: page.height/8
            text: "Отметки"
            anchors.bottom: rowLayout.top
            anchors.bottomMargin: 50
            anchors.right: parent.right
            anchors.rightMargin: 40
            anchors.left: parent.left
            anchors.leftMargin: 40
            customButtonFontPointSize: 30
            Layout.minimumWidth: 220
            Layout.fillHeight: true
            focusPolicy: Qt.StrongFocus
            onReleased: {
                stackView.push(marksPage)
            }
        }

        CustomButton {
            id: minusButton
            y: 915
            width: (setButton.width / 2) - 20
            height: setButton.height
            text: "Пред."
            anchors.bottom: setButton.top
            anchors.bottomMargin: 30
            anchors.left: parent.left
            anchors.leftMargin: 30
            Layout.fillWidth: true
            focusPolicy: Qt.NoFocus
            Layout.fillHeight: true
            customButtonFontPointSize: 30
            onClicked: {
                backend.prevTrackMark()                
            }
        }

        CustomButton {
            id: plusButton
            x: 399
            y: 915
            width: (setButton.width / 2) - 20
            height: setButton.height
            text: "След."
            anchors.bottom: setButton.top
            anchors.bottomMargin: 30
            anchors.right: parent.right
            anchors.rightMargin: 30
            customButtonFontPointSize: 30
            Layout.fillWidth: true
            Layout.fillHeight: true
            focusPolicy: Qt.NoFocus
            onClicked: {
                backend.nextTrackMark()                
            }
        }

        CustomButton {
            id: setButton
            y: 1064
            height: page.height/8
            text: qsTr("Установить")
            anchors.right: parent.right
            anchors.rightMargin: 30
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30
            customButtonFontPointSize: 30
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter            
            SequentialAnimation {
                id: setButtonTextAnimation
                running: false
                loops: Animation.Infinite
                OpacityAnimator {target: setButton; from: 0.3; to: 1; duration: 500}
                OpacityAnimator {target: setButton; from: 1; to: 0.3; duration: 500}
            }
            onReleased: {
                backend.setTrackMark()
                stopBlinking()
            }
        }

        RowLayout {
            id: rowLayout
            x: 280
            y: 631
            width: 232
            height: 45
            anchors.bottom: columnLayout.top
            anchors.bottomMargin: 70
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            spacing: 10
            Layout.fillHeight: true
            Layout.fillWidth: true

            Text {
                id: speedValue
                color: "#ffffff"
                text: qsTr("0 км/ч")
                font.pixelSize: 70
                font.bold: false
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter                
                Layout.fillWidth: false
                SequentialAnimation on color {
                    id: speedAnimation
                    loops: Animation.Infinite
                    ColorAnimation { from: "white"; to: "red"; duration: 500 }
                    ColorAnimation { from: "red"; to: "white"; duration: 500 }
                }
            }
        }

        ColumnLayout {
            id: columnLayout
            y: 452
            width: 151
            height: 58
            anchors.bottom: nextValueBackground.top
            anchors.bottomMargin: 70
            anchors.horizontalCenterOffset: 1
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter            

            Text {
                id: currentCoordinate
                width: 167
                height: 48
                color: "#ffffff"
                text: "0 km"
                font.pixelSize: page.height / 19
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
        }
    }

    Connections {
        target: backend
        onDoSocketConnected: {
            connectionStatusImage.source = "images/connected_72.png"
            connectionAnimation.stop()
            connectionStatusImage.opacity = 1
        }
        onDoSocketDisconnected: {
            connectionStatusImage.source = "images/disconnected_72.png"
            connectionAnimation.start()

            stopBlinking()
            registrationAnimation.stop()
            registrationStatusImage.source = "images/stop_rec_72.png"
            registrationStatusImage.opacity = 1
            speedValue.text = "0 км/ч"
        }
        onDoCurrentMeter: {
            mValue.text = m
        }
        onDoStartRegistration: {
            registrationStatusImage.source = "images/start_rec_72.png"
            registrationAnimation.start()
        }
        onDoStopRegistration: {
            registrationAnimation.stop()
            registrationStatusImage.source = "images/stop_rec_72.png"
            registrationStatusImage.opacity = 1
        }
        onDoIncrease: {
            registrationOptionImage.source = "images/increase_72.png"
        }
        onDoDecrease: {
            registrationOptionImage.source = "images/decrease_72.png"
        }
        onDoCurrentSpeed: {
            speedValue.text = Number(speed).toPrecision(((speed < 10) ? 2 : 3)) + " км/ч"
            if (speed >= 30) {
                speedValue.color = "red"                                
                speedAnimation.start()
            }
            else {
                speedValue.color = "white"                                                
                speedAnimation.stop()
                rowLayout.opacity = 1
            }
        }
        onDoCurrentTrackMark: {
            currentCoordinate.text = value
        }
        onDoNextTrackMarks: {
            nextValue.text = value
            nextValueAnimation.stop()
            nextValue.opacity = 1
        }
        onDoSatellitesFound: {
            satellitesImage.source = "images/satellites_found_72.png"
            sateliteAnimation.stop()
            satellitesImage.opacity = 1
        }
        onDoSatellitesNotFound: {
            satellitesImage.source = "images/satellites_notfound_72.png"
            sateliteAnimation.start()
            satellitesCount.text = 0
        }
        onDoSatellitesCount: {
            satellitesCount.text = count
        }
        onDoNotForgetGraphicsNotify: {
            nextValueAnimation.start();
            setButtonTextAnimation.start();
            nextValueBackground.border.width = 50
        }
    }
}
