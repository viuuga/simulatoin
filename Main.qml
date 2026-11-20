import QtQuick
import QtQuick.Controls
import App.Vulkan 1.0
import "qml/WindowComponents"
import "qml/Pages/generalPage"
import "Constants"

Rectangle {
    id: rootWindow
    width: 640
    height: 480
    color: Colors.rootWindowColor

    property bool gameActive: false

    readonly property int cornerRadius: 10
    readonly property bool windowDecorated: Window.window ? Window.window.visibility
                                                            !== Window.FullScreen
                                                            && Window.window.visibility
                                                            !== Window.Maximized : true

    radius: windowDecorated ? cornerRadius : 0

    layer.enabled: false
    layer.samples: 0

    function pushPage(url) {
        var props = {}
        if (url.indexOf("savesPage/SavesPage.qml") !== -1)
            props.onCreateWorld = rootWindow.openVkWorld
        stackView.push(url, props)
    }

    function openVkWorld() {
        stackView.clear()
        gameActive = true
        Qt.callLater(function() {
            vkItem.setMouseCaptured(true)
            vkItem.forceActiveFocus()
        })
    }

    function showGameMenu() {
        if (gameActive && stackView.depth === 0) {
            vkItem.setMouseCaptured(false)
            var menu = stackView.push("qrc:/qml/Interphase/gameMenu/gameMenu.qml", { onExitToMain: rootWindow.exitToMainMenu }, StackView.Immediate)
            if (menu)
                menu.forceActiveFocus()
        }
    }

    function resumeGame() {
        if (gameActive) {
            stackView.clear()
            Qt.callLater(function() {
                vkItem.setMouseCaptured(true)
                vkItem.forceActiveFocus()
            })
        }
    }

    function exitToMainMenu() {
        vkItem.setMouseCaptured(false)
        gameActive = false
        stackView.clear()
        stackView.push(Qt.resolvedUrl("qrc:/qml/Pages/generalPage/GeneralPage.qml"), { requestNavigationHandler: rootWindow.pushPage })
    }

    TopWindowToolbar {
        id: topWindowToolbar

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }

    WindowResizebleItem {
        visible: windowDecorated
    }

    Rectangle {
        id: vkHost
        visible: gameActive
        z: 0
        color: "#0d0d14"
        focus: gameActive

        anchors {
            top: topWindowToolbar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        VulkanItem {
            id: vkItem
            anchors.fill: parent
        }

        Keys.onEscapePressed: function(event) {
            rootWindow.showGameMenu()
            event.accepted = true
        }
    }

    StackView {
        id: stackView
        z: 10
        anchors {
            top: topWindowToolbar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        initialItem: GeneralPage {
            onRequestNavigation: (url) => rootWindow.pushPage(url)
        }
    }



    SequentialAnimation {
        id: hideToolbar

        ScriptAction {
            script: topWindowToolbar.hideButtons()
        }

        PauseAnimation {
            duration: 100
        }

        ParallelAnimation {
            NumberAnimation {
                target: topWindowToolbar
                property: "opacity"
                duration: 450
                easing.type: Easing.InOutQuad

                to: 0
            }

            NumberAnimation {
                target: topWindowToolbar
                property: "height"
                duration: 500
                easing.type: Easing.InOutQuad

                to: 0
            }
        }
    }

    ParallelAnimation {
        id: showToolbar

        ParallelAnimation {
            NumberAnimation {
                target: topWindowToolbar
                property: "opacity"
                duration: 450
                easing.type: Easing.InOutQuad

                to: 1
            }

            NumberAnimation {
                target: topWindowToolbar
                property: "height"
                duration: 500
                easing.type: Easing.InOutQuad
                to: SizeConsts.windowTopToolbarHeight
            }
        }

        SequentialAnimation {

            PauseAnimation {
                duration: 200
            }

            ScriptAction {
                script: topWindowToolbar.showButtons()
            }
        }
    }

    Shortcut {
        sequence: "Escape"
        context: Qt.WindowShortcut
        enabled: gameActive && stackView.depth === 0
        onActivated: rootWindow.showGameMenu()
    }

    Shortcut {
        sequence: "F11"
        onActivated: {
            var w = rootWindow.Window.window
            if (w.visibility === Window.FullScreen) {
                w.showNormal()
                showToolbar.start()
            } else {
                w.showFullScreen()
                hideToolbar.start()
            }
        }
    }
}
