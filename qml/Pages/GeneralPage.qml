import QtQuick
import "../../Constants"

Item {
    id: generalPage

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
            onClicked: StackView.view.push("qml/Pages/SavesPage.qml")
        }

        PagesButtons {
            id: btnOptions
            text: "Настройки"
            onClicked: StackView.view.push("qml/Pages/OptionsPage.qml")
        }

        PagesButtons {
            id: btnNetwork
            text: "Сеть"
            onClicked: {
                // пустышка - сеть в разработке
            }
        }
    }
}
