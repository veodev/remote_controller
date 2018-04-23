import QtQuick 2.0
import QtQuick.Controls 2.2

Button {
    id: setButton
    property alias customButtonText: setButton.text

    contentItem: Text {
        text: setButton.text
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
}


