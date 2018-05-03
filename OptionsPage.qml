import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Page {
    id: page
//    width: 720
//    height: 1280
    title: qsTr("Настройки")

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
                horizontalAlignment: Text.AlignHCenter
                validator: RegExpValidator {
                    regExp:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                }
                text: backend._ipAddress
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
            anchors.top: rowLayout.bottom
            anchors.topMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
        }

        RowLayout {
            id: rowLayout
            height: 165
            anchors.top: line1.bottom
            anchors.topMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20

            RowLayout {
                id: tumblerLayout
                width: 337
                height: 205
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.margins: 10
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
                    Layout.fillHeight: true
                    Layout.fillWidth: false
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

            RowLayout {
                id: soundSwitchLayout
                width: 341
                height: 56
                anchors.left: parent.left
                anchors.leftMargin: 20

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

        }
    }
}
