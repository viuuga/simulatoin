import QtQuick
import "../../../Constants"

Rectangle {
    id: button

    property string text: ""
    property color categoryColor: "#60529C8D"
    property bool active: false
    property real columnWidth: 200

    signal clicked()

    readonly property real edgeGap: columnWidth / 8
    readonly property int buttonHeight: 60

    width: active ? columnWidth : columnWidth * 7 / 8
    height: buttonHeight
    x: (active || !hoverArea.containsMouse) ? 0 : edgeGap
    z: active ? 2 : 1

    radius: active ? (height / 5) : (height / 5)

    topLeftRadius: radius
    bottomLeftRadius: radius
    topRightRadius: active ? 0 : radius
    bottomRightRadius: active ? 0 : radius

    color: (active || hoverArea.containsMouse) ? categoryColor : Colors.rootWindowColor

    Behavior on width {
        NumberAnimation { duration: 200; easing.type: Easing.OutQuad }
    }
    Behavior on x {
        NumberAnimation { duration: 200; easing.type: Easing.OutQuad }
    }
    Behavior on color {
        ColorAnimation { duration: 200; easing.type: Easing.OutQuad }
    }

    Text {
        anchors.centerIn: parent
        text: button.text
        color: "white"
        font.pixelSize: 16
    }

    MouseArea {
        id: hoverArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: button.clicked()
    }
}
