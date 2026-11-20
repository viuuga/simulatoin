import QtQuick
import QtQuick.Controls
import "../../../Constants"

Item {
    id: generalPage

    signal requestNavigation(string pageUrl)
    property var requestNavigationHandler: null

    function navigate(url) {
        if (requestNavigationHandler)
            requestNavigationHandler(url)
        else
            requestNavigation(url)
    }

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
            onClicked: generalPage.navigate("qrc:/qml/Pages/savesPage/SavesPage.qml")
        }

        PagesButtons {
            id: btnOptions
            text: "Настройки"
            onClicked: generalPage.navigate("qrc:/qml/Pages/settingsPage/SettingsPage.qml")
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