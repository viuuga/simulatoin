import QtQuick
import QtQuick.Controls
import "../../../Constants"

Rectangle {
    id: panel

    property var activeCategory: null
    property QtObject store: null

    property color panelColor: {
        if (!panel.activeCategory) return Colors.rootWindowColor
        switch (panel.activeCategory) {
            case "controls": return "#67CF9C"
            case "graphics": return "#CF804E"
            case "display": return "#A186D9"
            case "interface": return "#D986AD"
            default: return Colors.rootWindowColor
        }
    }

    color: panel.panelColor
    z: 0

    Behavior on color {
        ColorAnimation { duration: 250; easing.type: Easing.OutQuad }
    }

    Flickable {
        id: flick
        anchors.fill: parent
        anchors.leftMargin: Math.max(12, Math.round(parent.width * 0.04))
        anchors.rightMargin: Math.max(12, Math.round(parent.width * 0.04))
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        clip: true
        interactive: panel.activeCategory !== null

        ScrollBar.vertical: ScrollBar {
            width: 8
            policy: ScrollBar.AsNeeded
        }
    }
}
