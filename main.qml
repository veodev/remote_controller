import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4

Window {
    id: window
    visible: true
    width: 480
    height: 800
//    width: Screen.desktopAvailableWidth
//    height: Screen.desktopAvailableHeight
    color: "#1d325e"
    opacity: 1

    onFocusObjectChanged: {
        console.log("W: " + Screen.desktopAvailableWidth + " H: " + Screen.desktopAvailableHeight)
    }

    Drawer {
        id: drawer
        width: window.width * 0.66
        height: window.height
        background: Rectangle {
            color: "white"
        }

        Column {
            id: column
            anchors.fill: parent
            ItemDelegate {
                id: connectItem
                text: qsTr("Главная")
                width: parent.width
                onClicked: {
                    drawer.close()
                    view.currentIndex = 0
                }
            }

            ItemDelegate {
                id: optionsPageItem
                text: qsTr("Настройки")
                width: parent.width
                onClicked: {
                    drawer.close()
                    view.currentIndex = 1
                }
            }

            ItemDelegate {
                id: exitItem
                text: qsTr("Выход")
                width: parent.width
                onClicked: {
                    window.close()
                }
            }

        }
    }

    SwipeView {
        id: view
        currentIndex: 0
        anchors.fill: parent
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Item {
            MainPage{
                anchors.fill:parent
            }
        }
        Item {
            OptionsPage {
                anchors.fill:parent
            }
        }
    }
}
