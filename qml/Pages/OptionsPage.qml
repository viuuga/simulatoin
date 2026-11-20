import QtQuick
import "../../Constants"

Item {
    id: optionsPage

    Text {
        text: "Настройки"
        anchors.centerIn: parent
        color: "white"
        font.pixelSize: 24
    }

    Text {
        text: "Здесь будут все возможные настройки (сейчас пусто)"
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 40
        }
        color: "lightgray"
        font.pixelSize: 14
    }

    MouseArea {
        id: backArea
        anchors.fill: parent
        onClicked: StackView.view.pop()
    }
}
