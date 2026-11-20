import QtQuick
import Qt.labs.folderlistmodel 1.0
import "../../../Constants"

Item {
    id: savesPage

    property string savesPath: "/home/viuuga/projects/myProjects/simulation/project/data/saves"


    property var onBack: null
    property var onPushPage: null
    property var onCreateWorld: null


    Component.onCompleted: {
        if (!stackView) {
            var p = parent;
            while (p && typeof p.push !== 'function') {
                p = p.parent;
            }
            stackView = p;
        }
    }

    function goBack() {
        if (typeof onBack === 'function') {
            onBack();
        } else if (stackView) {
            stackView.pop();
        }
    }

    function pushPage(page) {
        if (typeof onPushPage === 'function') {
            onPushPage(page);
        } else if (stackView) {
            stackView.push(page);
        }
    }

    Item {
        id: leftPanel
        width: 300

        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }


        Column {
            id: topButtons
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            spacing: Spaces.buttonSpacingSmall

            LeftMenuButton {
                text: "Запустить мир"

                anchors{
                    left: parent.left
                    right: parent.right
                }
                onClicked: if (onCreateWorld) onCreateWorld()
            }

            LeftMenuButton {
                text: "Удалить мир"
                anchors{
                    left: parent.left
                    right: parent.right
                }
            }

            LeftMenuButton {
                text: "Пересоздать мир"
                anchors{
                    left: parent.left
                    right: parent.right
                }
            }

            LeftMenuButton {
                text: "Дублировать мир"
                anchors{
                    left: parent.left
                    right: parent.right
                }
            }

            Rectangle {
                id: separator
                width: parent.width
                height: 1
                color: "#666666"
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }

            LeftMenuButton {
                text: "Создать мир"
                anchors{
                    left: parent.left
                    right: parent.right
                }
                onClicked: if (onCreateWorld) onCreateWorld()
            }
        }


        LeftMenuButton {
            text: "Назад"
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            onClicked: goBack()
        }
    }


    Item {
        id: rightPanel

        anchors {
            top: parent.top
            left: leftPanel.right
            right: parent.right
            bottom: parent.bottom
        }

    }
}
