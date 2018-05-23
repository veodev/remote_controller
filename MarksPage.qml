import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

Page {
    id: marksPage
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

        CustomButton {
            id: startSwitchButton
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
            }
        }

        CustomButton {
            id: endSwitchButton
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
            }
        }

        TabBar {
            id: tabBar
            position: TabBar.Header
            z: 2
            anchors.top: startSwitchButton.bottom
            anchors.topMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20

            TabButton {
                id: bridgesTabButton
                text: qsTr("Мосты")
                onReleased: {
                    bridgesItem.visible = true
                    platformsItem.visible = false
                    miscItem.visible = false
                }
            }

            TabButton {
                id: platformsTabButton
                x: 201
                y: 0
                text: qsTr("Платформы")
                onReleased: {
                    bridgesItem.visible = false
                    platformsItem.visible = true
                    miscItem.visible = false
                }
            }

            TabButton {
                id: miscTabButton
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
                            anchors.fill: parent
                            anchors.margins: 10
                            text: buttonText
                            onReleased: {
                                backend.bridgeSelected(buttonText)
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
                            anchors.fill: parent
                            anchors.margins: 10
                            text: buttonText
                            onReleased: {
                                backend.platformSelected(buttonText)
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
                            anchors.fill: parent
                            anchors.margins: 10
                            text: buttonText
                            onReleased: {
                                backend.miscSelected(buttonText)
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
            bridgesListModel.append({buttonText: name})            
        }
        onDoAddItemToPlatformsModel: {
            platformsListModel.append({buttonText: name})            
        }
        onDoAddItemToMiscModel: {
            miscListModel.append({buttonText: name})            
        }
    }
}
