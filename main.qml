import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    id: window
    visible: true
    width: 720
    height: 1280

    MainPage {id: mainPage; visible: false;}
    MarksPage {id: marksPage; visible: false;}
    OptionsPage {id: optionsPage; visible: false;}

    header: ToolBar {
        contentHeight: toolButton.implicitHeight
        background: Rectangle {
            color: "#08104d"
        }

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6            
            onClicked: {
                (stackView.depth > 1) ? stackView.pop() : drawer.open()
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
            color: "white"
            font.pointSize: 16
        }
    }

    StackView {
        id: stackView
        initialItem: mainPage
        anchors.fill: parent
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
                    stackView.pop()
                    drawer.close()                    
                }
            }

            ItemDelegate {
                id: optionsPageItem
                text: qsTr("Настройки")
                width: parent.width
                onClicked: {
                    stackView.push(optionsPage)
                    drawer.close()                    
                }
            }

            ItemDelegate {
                id: startRegistrationItem
                text: qsTr("Включить\nрегистрацию")
                width: parent.width
                onClicked: {
                    drawer.close()
                    backend.startRegistration()
                }
            }

            ItemDelegate {
                id: stopRegistrationItem
                text: qsTr("Выключить\nрегистрацию")
                width: parent.width
                onClicked: {
                    drawer.close()
                    backend.stopRegistration()
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
}
