import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: window
    visible: true
    width: 720
    height: 1280

    property bool isClose: false

    MainPage {id: mainPage; visible: false;}
    MarksPage {id: marksPage; visible: false; }
    OptionsPage {id: optionsPage; visible: false;}

    header: ToolBar {
        contentHeight: toolButton.implicitHeight
        background: Rectangle {
            color: "#08104d"
        }

        ToolButton {
            id: toolButton
            implicitWidth: 100
            implicitHeight: 100
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                (stackView.depth > 1) ? stackView.pop() : drawer.open()
            }
            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                source: stackView.depth > 1 ? "images/back_72.png" : "images/menu_72.png"
            }

            background: Rectangle {
                border.color: "white"
                border.width: 2
                color: toolButton.down ? "white" : "#00000000"
                radius: 5
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
                font.pointSize: 20
                width: parent.width
                onClicked: {
                    stackView.pop()
                    drawer.close()                    
                }
            }

            ItemDelegate {
                id: optionsPageItem
                text: qsTr("Настройки")
                font.pointSize: 20
                width: parent.width
                onClicked: {
                    stackView.push(optionsPage)
                    drawer.close()                    
                }
            }

            ItemDelegate {
                id: startRegistrationItem
                text: qsTr("Включить\nрегистрацию")
                font.pointSize: 20
                width: parent.width
                onClicked: {
                    drawer.close()
                    backend.startRegistration()
                }
            }

            ItemDelegate {
                id: stopRegistrationItem
                text: qsTr("Выключить\nрегистрацию")
                font.pointSize: 20
                width: parent.width
                onClicked: {
                    drawer.close()
                    backend.stopRegistration()
                }
            }

            ItemDelegate {
                id: exitItem
                text: qsTr("Выход")
                font.pointSize: 20
                width: parent.width
                onClicked: {
                    isClose = true
                    window.close()
                }
            }
        }
    }

    MessageDialog {
        text: "Выйти из приложения?"
        id: dialog
        icon: StandardIcon.Question
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            isClose = true
            window.close()
        }
        onNo: dialog.close()
    }

    onClosing: {
        close.accepted = false
        if (isClose === false) {
            dialog.open()
        }
        else
            close.accepted = true
    }
}
