import QtQuick
import QtQuick.Controls
import "qml/WindowComponents"
import "qml/Pages/generalPage"
import "Constants"

Rectangle {
    id: rootWindow
    width: 640
    height: 480
    color: Colors.rootWindowColor

    readonly property int cornerRadius: 10
    readonly property bool windowDecorated: Window.window ? Window.window.visibility
                                                            !== Window.FullScreen
                                                            && Window.window.visibility
                                                            !== Window.Maximized : true

    radius: windowDecorated ? cornerRadius : 0

    layer.enabled: true
    layer.samples: 8

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

    StackView {
            id: stackView
            anchors {
                top: topWindowToolbar.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            initialItem: GeneralPage {
                onRequestNavigation: (url) => {
                    stackView.push(url)
                }
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
