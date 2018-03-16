import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Item {
    Rectangle {
        id: rectangle
        anchors.fill: parent
        color: "#ffffff"
        ColumnLayout {
            id: column
            anchors.topMargin: 20
            spacing: 30
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            CustomButton {
                id: startRegistrationButton
                text: qsTr("Включить\nрегистрацию")
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: parent.width
                onClicked: {
                    backend.startRegistration()
                }
            }

            CustomButton {
                id: stopRegistrationButton
                text: qsTr("Выключить\nрегистрацию")
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: parent.width
                onClicked: {
                    backend.stopRegistration()
                }
            }

            CustomButton {
                id: exitButton
                text: qsTr("Выход")
                transformOrigin: Item.Center
                Layout.rowSpan: 1
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                width: parent.width
                onClicked: {
                    window.close()
                }
            }
        }

    }




}
