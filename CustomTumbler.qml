import QtQuick 2.0
import QtQuick.Controls 2.3

Tumbler {
      id: control
      Rectangle {
          anchors.fill: parent
          color:"white"
          border.color: "white"
          radius: 5
      }

      delegate: Text {
          text: modelData
          font: control.font
          horizontalAlignment: Text.AlignLeft
          verticalAlignment: Text.AlignVCenter
          opacity: 1.0 - Math.abs(Tumbler.displacement) / (control.visibleItemCount / 2)
      }

      Rectangle {
          anchors.horizontalCenter: control.horizontalCenter
          y: control.height * 0.4
          width: control.width
          height: 1
          color: "#17a81a"
      }

      Rectangle {
          anchors.horizontalCenter: control.horizontalCenter
          y: control.height * 0.6
          width: control.width
          height: 1
          color: "#17a81a"
      }
  }


