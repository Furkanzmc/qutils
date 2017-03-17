import QtQuick 2.7

Item {

    // ----- Public Properties ----- //
    property color color

    // ----- Private Properties ----- //
    property bool _statusBarVisible: true
    property bool _immersiveMode: false

    id: root

    ColorPalette {
        id: colorPalette
        width: SH.dp(50) * 6
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        onClicked: {
            mainWindow.nativeUtils.setStatusBarColor(color);
        }
    }

    Button {
        id: btnToggleStatusBar
        width: parent.width
        height: SH.dp(50)
        anchors {
            top: colorPalette.bottom
            topMargin: SH.dp(30)
            horizontalCenter: parent.horizontalCenter
        }
        color: root.color
        text: "Toggle Status Bar"
        onClicked: {
            root._statusBarVisible = !root._statusBarVisible;
            mainWindow.nativeUtils.setStatusBarVisible(root._statusBarVisible);
        }
    }

    Button {
        id: btnImmersive
        width: parent.width
        height: SH.dp(50)
        anchors {
            top: btnToggleStatusBar.bottom
            topMargin: SH.dp(15)
            horizontalCenter: parent.horizontalCenter
        }
        color: root.color
        text: "Toggle Immersive Mode"
        onClicked: {
            root._immersiveMode = !root._immersiveMode;
            mainWindow.nativeUtils.setImmersiveMode(root._immersiveMode);
        }
    }
}
