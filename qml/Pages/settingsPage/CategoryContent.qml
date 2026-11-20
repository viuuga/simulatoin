import QtQuick
import QtQuick.Controls
import "../../../Constants"

Item {
    id: content

    property string category: ""
    property var activeCategory: null
    property QtObject store: null

    readonly property bool shown: content.category === content.activeCategory

    width: parent.width
    height: shown ? implicitHeight : 0
    implicitHeight: shown ? rootColumn.implicitHeight : 0
    opacity: shown ? 1 : 0
    visible: shown

    Behavior on opacity {
        NumberAnimation { duration: 200; easing.type: Easing.OutQuad }
    }

    Column {
        id: rootColumn
        width: parent.width
        spacing: 18

        Text {
            text: {
                switch (content.category) {
                    case "controls": return "Управление"
                    case "graphics": return "Графика"
                    case "display": return "Экран"
                    case "interface": return "Интерфейс"
                    default: return ""
                }
            }
            color: "white"
            font.pixelSize: 22
            font.bold: true
        }

        // ---- Управление ----
        Column {
            width: parent.width
            spacing: 14
            visible: content.category === "controls"

            Row {
                spacing: 12
                width: parent.width
                Text { text: "Чувствительность мыши"; color: "white"; font.pixelSize: 14; width: 160 }
                Slider {
                    id: sensSlider
                    from: 0.0005; to: 0.01; stepSize: 0.0001
                    value: content.store ? content.store.mouseSensitivity : 0.0025
                    width: Math.min(220, parent.width - 260)
                    onMoved: if (content.store) content.store.mouseSensitivity = value
                }
                Text { text: sensSlider.value.toFixed(4); color: "#e0e0e0"; font.pixelSize: 12 }
            }

            CheckBox {
                text: "Инверсия оси Y"
                checked: content.store ? content.store.invertY : false
                onToggled: if (content.store) content.store.invertY = checked
            }

            CheckBox {
                text: "Вибрация"
                checked: content.store ? content.store.vibration : true
                onToggled: if (content.store) content.store.vibration = checked
            }

            Row {
                spacing: 12
                Text { text: "Переназначение клавиш"; color: "white"; font.pixelSize: 14; width: 160 }
                Button {
                    text: "W / S / A / D / Space / C"
                    onClicked: {}
                }
            }
        }

        // ---- Графика ----
        Column {
            width: parent.width
            spacing: 14
            visible: content.category === "graphics"

            Row {
                spacing: 12
                width: parent.width
                Text { text: "Пресет качества"; color: "white"; font.pixelSize: 14; width: 160 }
                ComboBox {
                    model: ["Низкий", "Средний", "Высокий", "Ультра"]
                    currentIndex: content.store ? ["Низкий", "Средний", "Высокий", "Ультра"].indexOf(content.store.qualityPreset) : 1
                    onActivated: if (content.store) content.store.qualityPreset = currentText
                    width: 160
                }
            }

            CheckBox {
                text: "Тени"
                checked: content.store ? content.store.shadows : true
                onToggled: if (content.store) content.store.shadows = checked
            }
            CheckBox {
                text: "Сглаживание"
                checked: content.store ? content.store.antialiasing : true
                onToggled: if (content.store) content.store.antialiasing = checked
            }

            Row {
                spacing: 12
                width: parent.width
                Text { text: "Дальность прорисовки"; color: "white"; font.pixelSize: 14; width: 160 }
                Slider {
                    id: distSlider
                    from: 50; to: 1000; stepSize: 10
                    value: content.store ? content.store.renderDistance : 500
                    width: Math.min(220, parent.width - 260)
                    onMoved: if (content.store) content.store.renderDistance = value
                }
                Text { text: Math.round(distSlider.value) + " м"; color: "#e0e0e0"; font.pixelSize: 12 }
            }

            CheckBox {
                text: "Частицы"
                checked: content.store ? content.store.particles : true
                onToggled: if (content.store) content.store.particles = checked
            }
        }

        // ---- Экран ----
        Column {
            width: parent.width
            spacing: 14
            visible: content.category === "display"

            Row {
                spacing: 12
                width: parent.width
                Text { text: "Режим окна"; color: "white"; font.pixelSize: 14; width: 160 }
                ComboBox {
                    model: ["Окно", "Полноэкранный", "Безрамочный"]
                    currentIndex: content.store ? ["Окно", "Полноэкранный", "Безрамочный"].indexOf(content.store.windowMode) : 0
                    onActivated: if (content.store) content.store.windowMode = currentText
                    width: 160
                }
            }
            Row {
                spacing: 12
                width: parent.width
                Text { text: "Разрешение"; color: "white"; font.pixelSize: 14; width: 160 }
                ComboBox {
                    model: ["640x480", "800x600", "1280x720", "1920x1080"]
                    currentIndex: content.store ? ["640x480", "800x600", "1280x720", "1920x1080"].indexOf(content.store.resolution) : 2
                    onActivated: if (content.store) content.store.resolution = currentText
                    width: 160
                }
            }
            CheckBox {
                text: "V-Sync"
                checked: content.store ? content.store.vsync : true
                onToggled: if (content.store) content.store.vsync = checked
            }
            Row {
                spacing: 12
                width: parent.width
                Text { text: "Яркость"; color: "white"; font.pixelSize: 14; width: 160 }
                Slider {
                    id: brightSlider
                    from: 0.2; to: 2.0; stepSize: 0.05
                    value: content.store ? content.store.brightness : 1.0
                    width: Math.min(220, parent.width - 260)
                    onMoved: if (content.store) content.store.brightness = value
                }
                Text { text: brightSlider.value.toFixed(2); color: "#e0e0e0"; font.pixelSize: 12 }
            }
            Row {
                spacing: 12
                width: parent.width
                Text { text: "Лимит FPS"; color: "white"; font.pixelSize: 14; width: 160 }
                Slider {
                    id: fpsSlider
                    from: 30; to: 240; stepSize: 1
                    value: content.store ? content.store.fpsLimit : 60
                    width: Math.min(220, parent.width - 260)
                    onMoved: if (content.store) content.store.fpsLimit = value
                }
                Text { text: Math.round(fpsSlider.value); color: "#e0e0e0"; font.pixelSize: 12 }
            }
        }

        // ---- Интерфейс ----
        Column {
            width: parent.width
            spacing: 14
            visible: content.category === "interface"

            Row {
                spacing: 12
                width: parent.width
                Text { text: "Язык"; color: "white"; font.pixelSize: 14; width: 160 }
                ComboBox {
                    model: ["Русский", "English"]
                    currentIndex: content.store ? ["Русский", "English"].indexOf(content.store.language) : 0
                    onActivated: if (content.store) content.store.language = currentText
                    width: 160
                }
            }
            Row {
                spacing: 12
                width: parent.width
                Text { text: "Масштаб UI"; color: "white"; font.pixelSize: 14; width: 160 }
                Slider {
                    id: scaleSlider
                    from: 0.5; to: 2.0; stepSize: 0.05
                    value: content.store ? content.store.uiScale : 1.0
                    width: Math.min(220, parent.width - 260)
                    onMoved: if (content.store) content.store.uiScale = value
                }
                Text { text: Math.round(scaleSlider.value * 100) + "%"; color: "#e0e0e0"; font.pixelSize: 12 }
            }
            CheckBox {
                text: "Субтитры"
                checked: content.store ? content.store.subtitles : true
                onToggled: if (content.store) content.store.subtitles = checked
            }
            CheckBox {
                text: "Подсказки"
                checked: content.store ? content.store.tooltips : true
                onToggled: if (content.store) content.store.tooltips = checked
            }
            Row {
                spacing: 12
                width: parent.width
                Text { text: "Размер шрифта"; color: "white"; font.pixelSize: 14; width: 160 }
                Slider {
                    id: fontSlider
                    from: 10; to: 24; stepSize: 1
                    value: content.store ? content.store.fontSize : 14
                    width: Math.min(220, parent.width - 260)
                    onMoved: if (content.store) content.store.fontSize = value
                }
                Text { text: Math.round(fontSlider.value) + " px"; color: "#e0e0e0"; font.pixelSize: 12 }
            }
        }
    }
}
