import QtQuick
import Qt.labs.folderlistmodel 1.0
import "../../Constants"

Item {
    id: savesPage

    property string savesPath: "../data/saves"

    FolderListModel {
        id: savesModel
        folder: savesPage.savesPath
        nameFilters: ["*"]
    }

    ListView {
        id: savesList
        anchors.fill: parent
        model: savesModel
        delegate: Rectangle {
            height: 40
            width: ListView.view.width
            color: Colors.buttonToolbarWindowColor
            radius: 5

            Text {
                text: model.fileName
                anchors.centerIn: parent
                color: "white"
                font.pixelSize: 14
            }
        }
    }

    MouseArea {
        id: backArea
        anchors.fill: parent
        onClicked: StackView.view.pop()
    }
}
