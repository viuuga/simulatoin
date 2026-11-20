import QtQuick
import "../../../Constants"

Item {
    id: settingsPage

    property var activeCategory: null
    readonly property real leftRatio: 0.2
    readonly property real rightRatio: 0.8
    readonly property color neutralColor: Colors.rootWindowColor

    QtObject {
        id: settingsStore

        property real mouseSensitivity: 0.0025
        property bool invertY: false
        property bool vibration: true

        property string qualityPreset: "Средний"
        property bool shadows: true
        property bool antialiasing: true
        property real renderDistance: 500
        property bool particles: true

        property string windowMode: "Окно"
        property string resolution: "1280x720"
        property bool vsync: true
        property real brightness: 1.0
        property real fpsLimit: 60

        property string language: "Русский"
        property real uiScale: 1.0
        property bool subtitles: true
        property bool tooltips: true
        property real fontSize: 14
    }

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
        x: leftColumn.width
        width: parent.width * settingsPage.rightRatio
        height: parent.height
        z: 1
        activeCategory: settingsPage.activeCategory
        store: settingsStore
    }
}
