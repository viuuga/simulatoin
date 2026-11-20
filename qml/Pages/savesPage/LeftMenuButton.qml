import QtQuick

Rectangle {
    id: leftmenubutton

    width: 100
    height: 60

    property string text: "none"

    signal clicked()

    color: {
        if (leftmenubuttonarea.pressed)

            return "#50D1C7AE"
        if (leftmenubuttonarea.containsMouse)
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
        id: leftmenubuttonarea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: leftmenubutton.clicked()
    }

    Behavior on color {
        ColorAnimation {
            duration: 450
            easing.type: Easing.OutQuad
        }
    }
}
