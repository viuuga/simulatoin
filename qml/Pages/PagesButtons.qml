import QtQuick
import "../../Constants"


Item{
    id: pagesButton

    signal clicked()

    property string text: ""
    readonly property real defaultWidth: 240
    readonly property real defaultHeight: 60

    width: defaultWidth
    height: defaultHeight

    Rectangle {
        id: buttonRect

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
        }

        width: {
            if (buttonArea.pressed)
                return defaultWidth * 1.3
            if (buttonArea.containsMouse)
                return defaultWidth * 1.4
            return defaultWidth
        }
        height: {
            if (buttonArea.pressed)
                return defaultHeight * 1.05
            if (buttonArea.containsMouse)
                return defaultHeight * 1.1
            return defaultHeight
        }

        radius: height / 5

        color: {
            if (buttonArea.pressed)
                return Qt.darker(Colors.buttonToolbarWindowColor, 1.2)
            if (buttonArea.containsMouse)
                return Qt.lighter(Colors.buttonToolbarWindowColor, 1.3)
            return Colors.buttonToolbarWindowColor
        }

        Behavior on width {
            NumberAnimation {
                duration: 150
                easing.type: Easing.OutQuad
            }
        }

        Behavior on height {
            NumberAnimation {
                duration: 150
                easing.type: Easing.OutQuad
            }
        }

        Behavior on color {
            ColorAnimation {
                duration: 150
                easing.type: Easing.OutQuad
            }
        }

        Text {
            anchors.centerIn: parent
            text: pagesButton.text
            color: "white"
            font.pixelSize: 16
        }

        MouseArea {
            id: buttonArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: pagesButton.clicked()
        }
    }
}
