import QtQuick
import QtQuick.Controls
import "../../../Constants"

Item {
    id: generalPage

    signal requestNavigation(string pageUrl)

    Column {
        anchors {
            top: parent.verticalCenter
            left: parent.left
            bottom: parent.bottom
            leftMargin: Spaces.edgesWindowMedium
            bottomMargin: Spaces.edgesWindowMedium
        }

        spacing: Spaces.buttonSpacingMedium

        PagesButtons {
            id: btnSaves
            text: "Сохранения"
            onClicked: generalPage.requestNavigation("qml/Pages/savesPage/SavesPage.qml")
        }

        PagesButtons {
            id: btnOptions
            text: "Настройки"
            onClicked: generalPage.requestNavigation("qml/Pages/optionsPage/OptionsPage.qml")
        }

        PagesButtons {
            id: btnNetwork
            text: "Сеть"
            onClicked: {
                // пустышка
            }
        }
    }
}