import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Page {
//    width: 720
//    height: 1280
    title: qsTr("Отметки")

    Rectangle {
        id: rectangle
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {position: 0; color: "#08104d"}
            GradientStop {position: 1; color: "#000000"}
        }

        Text {
            id: text1
            x: 252
            y: 31
            color: "#ffffff"
            text: qsTr("Служебные отметки")
            font.pixelSize: 20
        }

        TabBar {
            id: tabBar
            x: 155
            y: 137
            width: 348
            height: 261
            position: TabBar.Footer
            currentIndex: -1

            TabButton {
                id: tabButton
                text: qsTr("Tab Button")
            }

            TabButton {
                id: tabButton1
                text: qsTr("Tab Button")
            }
        }
    }
    Keys.onEscapePressed: {
        stackView.pop()
    }
}
