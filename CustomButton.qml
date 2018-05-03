import QtQuick 2.0
import QtQuick.Controls 2.2

Button {
    id: button
    property alias customButtonText: button.text
    contentItem: Text {
        text: button.text
        font.pointSize: 24
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: button.down ? "black" : "white"
    }

    background: Rectangle {
        border.color: "white"
        border.width: 2
        color: button.down ? "white" : "#00000000"
        radius: 5
    }
}


