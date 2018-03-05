import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import remote_controller.appcore 1.0
import Qt.labs.calendar 1.0

Window {
    id: window
    visible: true
    width: 480
    height: 800
    color: "#1d325e"
    opacity: 1

    AppCore {
        id: backend
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

        Item {
            id: mainPage
            MainPage {
                anchors.fill: parent
            }
        }
        Item {
            id: optionsPage
            OptionsPage {
            }
        }
    }

    PageIndicator {
        id: indicator
        count: view.count
        currentIndex: view.currentIndex
        anchors.bottom: view.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
