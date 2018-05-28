import QtQuick 2.0
import QtQuick.Controls 2.2

Button {
    id: control
    property alias customButtonText: control.text
    property alias customButtonFontPointSize: controlText.font.pointSize
    property int customButtonIndex: -1
    contentItem: Text {
        id: controlText
        text: control.text
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: control.down ? "black" : "white"
    }

    background: Rectangle {
        border.color: "white"
        border.width: 2
        color: control.down ? "white" : "#00000000"
        radius: 5
    }
}


