import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

Page {
    id: page
////    width: 720
////    height: 1280
//    anchors.fill: parent
    title: qsTr("Отметки")

    Rectangle {
        id: rectangle
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {position: 0; color: "#08104d"}
            GradientStop {position: 1; color: "#000000"}
        }

        CustomButton {
            id: startSwitchButton
            customButtonFontPointSize: 30
            text: "Начало\n стрелки"
            z: 2
            spacing: 5
            anchors.right: endSwitchButton.left
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            height: window.height/10
            onReleased: {
                backend.startSwitch()
                stackView.pop()
            }
        }

        CustomButton {
            id: endSwitchButton
            customButtonFontPointSize: 30
            x: 401
            text: "Конец\n стрелки"
            z: 2
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 20
            width: (window.width / 2) - 20
            height: window.height/10
            onReleased: {
                backend.endSwitch()
                stackView.pop()
            }
        }

        TabBar {
            id: tabBar
            height: window.height/20
            position: TabBar.Header
            z: 2
            anchors.top: startSwitchButton.bottom
            anchors.topMargin: 80
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20            

            TabButton {
                id: bridgesTabButton
                height: tabBar.height
                text: qsTr("Мосты")                
                onReleased: {
                    bridgesItem.visible = true
                    platformsItem.visible = false
                    miscItem.visible = false
                }
            }

            TabButton {
                id: platformsTabButton
                height: tabBar.height
                text: qsTr("Платформы")
                onReleased: {
                    bridgesItem.visible = false
                    platformsItem.visible = true
                    miscItem.visible = false
                }
            }

            TabButton {
                id: miscTabButton
                height: tabBar.height
                text: qsTr("Прочие")
                onReleased: {
                    bridgesItem.visible = false
                    platformsItem.visible = false
                    miscItem.visible = true
                }
            }

        }

        StackLayout {
            id: stackLayout
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: tabBar.bottom
            anchors.topMargin: 6
            currentIndex: tabBar.currentIndex

            Item {
                id: bridgesItem
                visible: false
                ListView {
                    id: bridgesList
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    flickableDirection: Flickable.VerticalFlick
                    boundsBehavior: Flickable.StopAtBounds
                    anchors.fill: parent
                    delegate: Item {
                        id: bridgeslistItem
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: window.height/10

                        CustomButton {
                            customButtonIndex: buttonIndex
                            customButtonFontPointSize: 20
                            anchors.fill: parent
                            anchors.topMargin: 60
                            anchors.leftMargin: 0
                            anchors.rightMargin: 0
                            text: buttonText
                            onReleased: {
                                backend.bridgeSelected(buttonIndex)
                                stackView.pop()
                            }
                        }
                    }
                    model: ListModel {
                        id: bridgesListModel
                    }
                }
            }
            Item {
                id: platformsItem
                visible: false
                ListView {
                    id: platformsList
                    flickableDirection: Flickable.VerticalFlick
                    boundsBehavior: Flickable.StopAtBounds
                    anchors.fill: parent
                    delegate: Item {
                        id: platformslistItem
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: window.height/10

                        CustomButton {
                            customButtonIndex: buttonIndex
                            customButtonFontPointSize: 20
                            anchors.fill: parent
                            anchors.topMargin: 60
                            anchors.leftMargin: 0
                            anchors.rightMargin: 0
                            text: buttonText
                            onReleased: {
                                backend.platformSelected(buttonIndex)
                                stackView.pop()
                            }
                        }
                    }
                    model: ListModel {
                        id: platformsListModel
                    }
                }
            }
            Item {
                id: miscItem
                visible: false
                ListView {
                    id: miscList
                    flickableDirection: Flickable.VerticalFlick
                    boundsBehavior: Flickable.StopAtBounds
                    anchors.fill: parent
                    delegate: Item {
                        id: miscListItem
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: window.height/10

                        CustomButton {
                            customButtonIndex: buttonIndex
                            customButtonFontPointSize: 20
                            anchors.fill: parent
                            anchors.topMargin: 60
                            anchors.leftMargin: 0
                            anchors.rightMargin: 0
                            text: buttonText
                            onReleased: {                                
                                backend.miscSelected(buttonIndex)
                                stackView.pop()
                            }
                        }
                    }
                    model: ListModel {
                        id: miscListModel
                    }
                }
            }
        }

        Rectangle {
            id: rectangle1
            x: 20
            y: 20
            width: 680
            height: 168
            color: "#08104d"
            z: 1
        }
    }

    Keys.onBackPressed: {
        stackView.pop()
    }
    Connections {
        target: backend
        onDoClearBridgesModel: {
            bridgesListModel.clear()            
        }
        onDoClearPlatformsModel: {
            platformsListModel.clear()            
        }
        onDoClearMiscModel: {
            miscListModel.clear()            
        }
        onDoAddItemToBridgesModel: {
            bridgesListModel.append({buttonText: name, buttonIndex: index})
        }
        onDoAddItemToPlatformsModel: {
            platformsListModel.append({buttonText: name, buttonIndex: index})
        }
        onDoAddItemToMiscModel: {
            miscListModel.append({buttonText: name, buttonIndex: index})
        }
    }
}
