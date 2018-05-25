import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Page {
    id: page
    width: 720
    height: 1280
    anchors.fill: parent
    title: qsTr("Настройки")

    function enableOptionsLayout(isEnable) {
        if (isEnable) {
            optionsNotifyLayout.opacity = 1
            optionsNotifyLayout.enabled = true
        }
        else {
            optionsNotifyLayout.opacity = 0.3
            optionsNotifyLayout.enabled = false
        }
    }

    Rectangle {
        id: rectangle
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0; color: "#08104d"}
            GradientStop {position: 1; color: "#000000"}
        }

        RowLayout {
            id: ipLayout
            anchors.top: parent.top
            anchors.topMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.margins: 10

            Label {
                color: "#ffffff"
                text: "IP-адрес:"
                horizontalAlignment: Text.AlignRight
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.fillWidth: false
                verticalAlignment: Text.AlignVCenter
                Layout.fillHeight: true
                font.pointSize: 20
            }

            TextField {
                id: ipAddressText
                Layout.fillHeight: true
                horizontalAlignment: Text.AlignLeft
                validator: RegExpValidator {
                    regExp:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                }
                text: backend._ipAddress
                topPadding: 10
                font.bold: true
                Layout.fillWidth: true
                onEditingFinished: backend._ipAddress = ipAddressText.text
            }
        }

        Rectangle {
            id: line1
            height: 2
            color: "#ffffff"
            radius: 1
            anchors.top: ipLayout.bottom
            anchors.topMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
        }

        Rectangle {
            id: line2
            height: 2
            color: "#ffffff"
            radius: 1
            anchors.top: optionsNotifyLayout.bottom
            anchors.topMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
        }

        RowLayout {
            id: optionsNotifyLayout
            y: 331
            height: 222
            anchors.top: statusNotifyLayout.bottom
            anchors.topMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 21
            anchors.left: parent.left
            anchors.leftMargin: 21

            ColumnLayout {
                id: columnLayout
                width: 100
                height: 100
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                Layout.fillWidth: false

                RowLayout {
                    id: soundSwitchLayout
                    width: 341
                    height: 56
                    Text {
                        id: text3
                        width: 198
                        height: 45
                        color: "#ffffff"
                        text: qsTr("Звук:")
                        font.pointSize: 20
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }

                    CustomSwitch {
                        id: notifySoundStatus                        
                        Layout.fillWidth: false
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Component.onCompleted: notifySoundStatus.checked = backend._isNotifySoundEnabled
                        onCheckedChanged: backend._isNotifySoundEnabled = notifySoundStatus.checked                        
                    }
                    anchors.leftMargin: 20
                }

                RowLayout {
                    id: graphicsSwitchLayout
                    width: 341
                    height: 56
                    anchors.leftMargin: 20

                    Text {
                        id: text1
                        width: 198
                        height: 45
                        color: "#ffffff"
                        text: qsTr("Графика:")
                        verticalAlignment: Text.AlignVCenter
                        Layout.fillWidth: false
                        Layout.fillHeight: true
                        horizontalAlignment: Text.AlignLeft
                        font.pointSize: 20
                    }

                    CustomSwitch {
                        id: notifyGraphicsStatus                        
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Component.onCompleted: notifyGraphicsStatus.checked = backend._isNotifyGraphicsEnabled
                        onCheckedChanged: backend._isNotifyGraphicsEnabled = notifyGraphicsStatus.checked
                    }
                }
            }

            RowLayout {
                id: tumblerLayout
                width: 337
                height: 205
                spacing: 20
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                anchors.rightMargin: 20
                anchors.margins: 10
                Label {
                    color: "#ffffff"
                    text: "Порог, м:"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: false
                    Layout.fillHeight: true
                    font.pointSize: 20
                }

                Tumbler {
                    id: notifyThreshold
                    height: 100
                    font.pointSize: 16
                    font.bold: true
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.fillHeight: true
                    Layout.fillWidth: false
                    focusPolicy: Qt.NoFocus
                    visibleItemCount: 5
                    model: ListModel {
                        Component.onCompleted: {
                            for (var i = 5; i <= 50; i += 5) {
                                append({value: i.toString()});
                            }
                            notifyThreshold.currentIndex = backend._notifyThresholdIndex
                        }
                    }
                    background: Rectangle {
                        color:"white"
                        border.color: "white"
                        radius: 5
                    }

                    onCurrentIndexChanged: {
                        backend._notifyThresholdIndex = notifyThreshold.currentIndex
                        backend.setNotifyThreshold(parseInt(currentItem.text))
                    }
                }
            }
            Component.onCompleted: {
                enableOptionsLayout(notifyStatusSwitch.checked)
            }
        }

        Label {
            id: notifyLabel
            x: 39
            width: 447
            height: 50
            color: "#ffffff"
            text: qsTr("Напоминание о координате:")
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.top: line1.bottom
            anchors.topMargin: 40
            font.pointSize: 20
        }

        RowLayout {
            id: statusNotifyLayout
            x: 190
            width: 341
            height: 106
            anchors.top: notifyLabel.bottom
            anchors.topMargin: 40
            Text {
                id: text2
                width: 198
                height: 45
                color: "#ffffff"
                text: qsTr("Состояние:")
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                font.pointSize: 20
                Layout.fillWidth: false
                Layout.fillHeight: true
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }

            CustomSwitch {
                id: notifyStatusSwitch
                Component.onCompleted: notifyStatusSwitch.checked = backend._isNotifyEnabled                
                onCheckedChanged: {
                    backend._isNotifyEnabled = notifyStatusSwitch.checked
                    enableOptionsLayout(notifyStatusSwitch.checked)
                }

            }
            anchors.leftMargin: 20
        }
    }
}
