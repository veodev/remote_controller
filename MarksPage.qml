import QtQuick 2.6
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

Page {
    id: marksPage
    width: 720
    height: 1280
    title: qsTr("Отметки")

    //    signal marksSelected(String name)

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
            spacing: 5
            anchors.right: endSwitchButton.left
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            height: window.height/10
        }

        CustomButton {
            id: endSwitchButton
            x: 401
            text: "Конец\n стрелки"
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 20
            width: (window.width / 2) - 20
            height: window.height/10
        }

        TabBar {
            id: tabBar
            position: TabBar.Header
            anchors.top: startSwitchButton.bottom
            anchors.topMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20

            TabButton {
                id: bridgesTabButton
                text: qsTr("Мосты")
                onPressed: {
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
                onPressed: {
                    bridgesItem.visible = false
                    platformsItem.visible = true
                    miscItem.visible = false
                }
            }

            TabButton {
                id: miscTabButton
                text: qsTr("Прочие")
                onPressed: {
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
            anchors.topMargin: 10
            currentIndex: tabBar.currentIndex

            Item {
                id: bridgesItem
                visible: false
                ListView {
                    id: bridgesList
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
                                backend.marksSelected(buttonText)
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
                                backend.marksSelected(buttonText)
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
                                backend.marksSelected(buttonText)
                            }
                        }
                    }
                    model: ListModel {
                        id: miscListModel
                    }
                }
            }
        }
    }

    Keys.onBackPressed: {
        stackView.pop()
    }
    Connections {
        target: backend
        onClearBridgesModel: {
            bridgesListModel.clear()
            console.log("Clear bridges model")
        }
        onClearPlatformsModel: {
            platformsListModel.clear()
            console.log("Clear platforms model")
        }
        onClearMiscModel: {
            miscListModel.clear()
            console.log("Clear misc model")
        }
        onAddItemToBridgesModel: {
            bridgesListModel.append({buttonText: name})
            console.log("Add to bridges: ", name)
        }
        onAddItemToPlatformsModel: {
            platformsListModel.append({buttonText: name})
            console.log("Add to platforms: ", name)
        }
        onAddItemToMiscModel: {
            miscListModel.append({buttonText: name})
            console.log("Add to misc: ", name)
        }
    }
}
