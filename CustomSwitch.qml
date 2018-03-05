import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Switch {
    id: control
    text: qsTr("Switch")

    indicator: Rectangle {
        implicitWidth: 80
        implicitHeight: 40
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: 20
        color: control.checked ? "green" : "red"

        Rectangle {
            x: control.checked ? parent.width - width : 0
            width: 40
            height: 40
            radius: 20
            color: control.down ? "#cccccc" : "#ffffff"
            border.color: control.checked ? (control.down ? "#17a81a" : "#21be2b") : "#999999"
        }
    }

    contentItem: Text {
        color: "#00000000"
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        verticalAlignment: Text.AlignVCenter
    }
}

