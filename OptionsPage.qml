import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import remote_controller.appcore 1.0

Item {
    id: item1
    width: 480
    height: 800

    Rectangle {
        id: rectangle
        color: "#1d325e"
        anchors.fill: parent

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
            }
        }

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

        CustomSwitch {
            id: switch1
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: false
            Layout.fillHeight: true
            checkable: true
            hoverEnabled: true
            focusPolicy: Qt.NoFocus

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
