import QtQuick
import "../../../Constants"

Item {
    id: settingsPage

    property var activeCategory: null
    readonly property real leftRatio: 0.2
    readonly property real rightRatio: 0.8
    readonly property color neutralColor: Colors.rootWindowColor

    function selectCategory(cat) {
        if (activeCategory === cat)
            return
        activeCategory = cat
    }

    Item {
        id: leftColumn
        width: parent.width * settingsPage.leftRatio
        height: parent.height
        z: 2

        Column {
            anchors.centerIn: parent
            spacing: Spaces.buttonSpacingSmall

            SettingsCategoryButton {
                text: "Управление"
                categoryColor: "#67CF9C"
                columnWidth: leftColumn.width
                active: settingsPage.activeCategory === "controls"
                onClicked: settingsPage.selectCategory("controls")
            }

            SettingsCategoryButton {
                text: "Графика"
                categoryColor: "#CF804E"
                columnWidth: leftColumn.width
                active: settingsPage.activeCategory === "graphics"
                onClicked: settingsPage.selectCategory("graphics")
            }

            SettingsCategoryButton {
                text: "Экран"
                categoryColor: "#A186D9"
                columnWidth: leftColumn.width
                active: settingsPage.activeCategory === "display"
                onClicked: settingsPage.selectCategory("display")
            }

            SettingsCategoryButton {
                text: "Интерфейс"
                categoryColor: "#D986AD"
                columnWidth: leftColumn.width
                active: settingsPage.activeCategory === "interface"
                onClicked: settingsPage.selectCategory("interface")
            }
        }
    }

    SettingsPanel {
        id: panel

        anchors {
            top: parent.top
            left: leftColumn.right
            right: parent.right
            bottom: parent.bottom

            topMargin: Spaces.panelSpasingMedium
            rightMargin: Spaces.panelSpasingMedium
            bottomMargin: Spaces.panelSpasingMedium
        }
        radius: 8

        activeCategory: settingsPage.activeCategory

    }
}
