import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import remote_controller.appcore 1.0
import Qt.labs.settings 1.0

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
                text: qsTr("Главная")
                width: parent.width
                onClicked: {
                    drawer.close()
                    view.currentIndex = 0
                }
            }

            ItemDelegate {
                id: optionsPageItem
                text: qsTr("Настройки")
                width: parent.width
                onClicked: {
                    drawer.close()
                    view.currentIndex = 1
                }
            }

            ItemDelegate {
                id: exitItem
                text: qsTr("Выход")
                width: parent.width
                onClicked: {
                    window.close()
                }
            }

        }
    }

    SwipeView {
        id: view
        currentIndex: 0
        anchors.fill: parent

        Item {
            id: mainPage
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
                    text: qsTr("6/7 пк")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 50
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

                    onClicked: {
                        backend.prevTrackmark()
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
                        backend.nextTrackmark()
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

                onDoNewData: {
                    kmValue.text = km
                    pkValue.text = pk
                    mValue.text = m
                }

                onDoCurrentMeterAndSpeed: {
                    mValue.text = m
                }

                onDoStartRegistration: {
                    registrationStatusImage.source = "images/start_rec.png"
                    kmValue.text = km
                    pkValue.text = pk
                    mValue.text = m
                }

                onDoStopRegistration: {
                    kmValue.text = "-"
                    pkValue.text = "-"
                    mValue.text = "-"
                    registrationStatusImage.source = "images/stop_rec.png"
                }

                onDoIncrease: {
                    registrationOptionImage.source = "images/increase.png"
                }

                onDoDecrease: {
                    registrationOptionImage.source = "images/decrease.png"
                }                
            }



        }
        Item {
            id: optionsPage
            Rectangle {
                id: rectangle
                color: "#1d325e"
                anchors.fill: parent

                Text {
                    id: text2
                    x: 149
                    y: 29
                    width: 196
                    height: 20
                    color: "#ffffff"
                    text: qsTr("Настройки")
                    font.pointSize: 20
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                RowLayout {
                    y: 78
                    anchors.horizontalCenterOffset: 2
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.margins: 10

                    Label {
                        color: "#ffffff"
                        text: "IP-адрес:"
                        verticalAlignment: Text.AlignVCenter
                        Layout.fillHeight: true
                        font.pointSize: 20
                    }

                    TextField {
                        id: ipAddressText
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        validator: RegExpValidator {
                            regExp:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                        }
                        text: backend._ipAddress
                        onEditingFinished: backend._ipAddress = ipAddressText.text
                    }
                }


                Rectangle {
                    id: rectangle1
                    x: 6
                    y: 142
                    width: 464
                    height: 2
                    color: "#ffffff"
                    radius: 1
                }

                Rectangle {
                    id: rectangle2
                    x: 8
                    y: 497
                    width: 464
                    height: 2
                    color: "#ffffff"
                    radius: 1
                }
            }

            RowLayout {
                id: row
                x: 65
                y: 175
                width: 341
                height: 56
                anchors.horizontalCenterOffset: 1
                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    id: text1
                    width: 198
                    height: 45
                    color: "#ffffff"
                    text: qsTr("Зуммер:")
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: false
                    Layout.fillHeight: true
                    horizontalAlignment: Text.AlignLeft
                    font.pointSize: 20
                }

                Switch {
                    id: soundSwitch
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: false
                    Layout.fillHeight: true
                    checkable: true
                    hoverEnabled: true
                    focusPolicy: Qt.NoFocus
                    checked: backend._isSoundEnabled
                    onClicked: backend._isSoundEnabled = soundSwitch.checked

                    indicator: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 50
                            x: soundSwitch.leftPadding
                            y: parent.height / 2 - height / 2
                            radius: 25
                            color: soundSwitch.checked ? "#17a81a" : "#ffffff"
                            border.color: soundSwitch.checked ? "#17a81a" : "#cccccc"

                            Rectangle {
                                x: soundSwitch.checked ? parent.width - width : 0
                                width: 50
                                height: 50
                                radius: 25
                                color: soundSwitch.down ? "#cccccc" : "#ffffff"
                                border.color: "black"
                            }
                        }

                        contentItem: Text {
                            text: soundSwitch.text
                            font: soundSwitch.font
                            opacity: enabled ? 1.0 : 0.3
                            color: soundSwitch.down ? "#17a81a" : "#21be2b"
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: soundSwitch.indicator.width + soundSwitch.spacing
                        }
                }
            }

            RowLayout {
                x: 74
                y: 266
                width: 337
                height: 200
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: 10
                anchors.horizontalCenterOffset: 3
                Label {
                    color: "#ffffff"
                    text: "Порог, м:"
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: false
                    Layout.fillHeight: true
                    font.pointSize: 20
                }

                Tumbler {
                    height: 100
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    focusPolicy: Qt.NoFocus
                    visibleItemCount: 3
                    model: 50
                    background: Rectangle {
                        color:"white"
                        border.color: "white"
                        radius: 5
                    }
               }
            }
        }
    }
}
