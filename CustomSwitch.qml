import QtQuick 2.6
import QtQuick.Controls 2.1

Switch {
    id: control
    text: control.checked ? "Вкл": "Выкл"

    indicator: Rectangle {
        width: 80
        height: 40
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: 20
        color: control.checked ? "#17a81a" : "#ffffff"
        border.color: control.checked ? "#17a81a" : "#cccccc"

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
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.checked ? "#17a81a" : "red"
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
    }
}
