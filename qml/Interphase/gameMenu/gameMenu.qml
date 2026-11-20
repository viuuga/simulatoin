import QtQuick

import QtQuick.Controls

Rectangle {
    id: gameMenu

    property var onExitToMain: null

    color: "#CC000000"
    focus: true
    z: 1

    Keys.onEscapePressed: function(event) { event.accepted = true }

    Component.onCompleted: {
        if (StackView.view) {
            width = StackView.view.width
            height = StackView.view.height
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 30

        Rectangle {
            width: 240
            height: 60
            color: "#60529C8D"
            radius: 10

            MouseArea {
                id: btnSettingsArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: console.log("Настройки - затычка")
            }

            Text {
                anchors.centerIn: parent
                text: "Настройки"
                color: "white"
                font.pixelSize: 16
            }
        }

        Rectangle {
            width: 240
            height: 60
            color: "#60529C8D"
            radius: 10

            MouseArea {
                id: btnMainMenuArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: if (onExitToMain) onExitToMain()
            }

            Text {
                anchors.centerIn: parent
                text: "В главное меню"
                color: "white"
                font.pixelSize: 16
            }
        }
    }
}
