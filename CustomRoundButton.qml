import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4

    Button {
        id: control
        property alias customText: control.text
        font.pixelSize: 30

        contentItem: Text {
            text: control.text
            font: control.font
            color: control.pressed ? "black" : "yellow"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle {
            color: control.pressed ? "yellow" : "black"
            border.color: "yellow"
            border.width: 2
            radius: 2
        }
    }

