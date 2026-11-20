import QtQuick

Rectangle {
    id: optionsPageButton

    width: 100
    height: 60

    property string text: "none"

    signal clicked()

    color: {
        if (optionsPageButtonArea.pressed)

            return "#50D1C7AE"
        if (optionsPageButtonArea.containsMouse)
            return "#30D1C7AE"
        return "transparent"
    }

    Text {
        anchors.centerIn: parent
        text: leftmenubutton.text
        color: "white"
        font.pixelSize: 16
    }

    MouseArea {
        id: optionsPageButtonArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: optionsPageButton.clicked()
    }

    Behavior on color {
        ColorAnimation {
            duration: 450
            easing.type: Easing.OutQuad
        }
    }
}
