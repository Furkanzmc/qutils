import QtQuick 2.7
import QtQuick.Window 2.2
import qutils.AndroidUtils 1.0

Window {

    property alias nativeUtils: androidUtils

    id: mainWindow
    width: 320
    height: 480
    title: "qutils Demo"
    color: "#FDFAFB"
    visible: true
    Component.onCompleted: {
        if (Q_OS_DESKTOP && QML_DEBUG) {
            QM.setWindow(mainWindow);
        }
    }

    AndroidUtils {
        id: androidUtils
    }

    StatusBarView {
        id: statusBarView
        anchors {
            fill: parent
            topMargin: menu.collapsedHeight + SH.dp(10)
        }
        color: mainWindow.color
        visible: false
    }

    NotificationsView {
        id: notificationsView
        anchors {
            fill: parent
            topMargin: menu.collapsedHeight + SH.dp(10)
        }
        color: mainWindow.color
    }

    Menu {
        id: menu
        width: parent.width
        height: parent.height
        collapsedHeight: mainWindow.height * 0.1
        collapsed: true

        MenuItem {
            color: "#D77EFC"
            selected: true
            collapsed: menu.collapsed
            index: 0
            title: "NOTIFICATIONS"
            onSelectedChanged: {
                notificationsView.visible = selected;
//                androidUtils.setStatusBarColor(color);
            }
        }

        MenuItem {
            color: "#FC6F61"
            collapsed: menu.collapsed
            index: 1
            title: "STATUS BAR"
            onSelectedChanged: {
                statusBarView.visible = selected;
//                androidUtils.setStatusBarColor(color);
            }
        }

        MenuItem {
            color: "#94C325"
            collapsed: menu.collapsed
            index: 2
            title: "REFRESH"
            onSelectedChanged: {
                if (selected) {
                    QM.reload();
                }
            }
        }
    }
}
