import QtQuick 2.7
import NotificationClient 1.0

Item {

    // ----- Public Properties ----- //

    property color color

    id: root

    NotificationClient {
        id: notificationClient
        objectName: "notificationClient"
        onNotificationReceived: {
            if (receivedTag === "instant_notification") {
                lbTitle.text = "Instant Notification is Received!";
            }
            else if (receivedTag === "5_second_tag") {
                lbTitle.text = "Scheduled Notification is Received After 5 Seconds!";
            }
            else if (receivedTag === "15_second_tag") {
                lbTitle.text = "Scheduled Notification is Received After 15 Seconds!";
            }
        }
    }

    Notification {
        id: notification
        delay: 1000
        text: "User is something!"
        title: "HEYO!"
        ledColor: Qt.rgba(123, 232, 12, 255)
        ledOnMS: 1000
        ledOffMS: 200
        category: CATEGORY_STATUS
        defaults: DEFAULT_LIGHTS
        flags: FLAG_AUTO_CANCEL
        priority: PRIORITY_MAX
    }

    Text {
        id: lbTitle
        anchors {
            top: parent.top
            topMargin: SH.dp(20)
            horizontalCenter: parent.horizontalCenter
        }
        font.pointSize: 15
        color: "#2196F3"
    }

    Column {
        id: cl
        width: parent.width
        height: SH.dp(50) * 3
        anchors {
            top: lbTitle.bottom
            topMargin: SH.dp(20)
            horizontalCenter: parent.horizontalCenter
        }

        Button {
            id: title
            color: root.color
            text: "Send Instant Notification"
            width: parent.width
            height: SH.dp(50)
            onClicked: {
                var notif = {
                    "delay": 0,
                    "text": "This is an instant notification. Tap on me to return to the app.",
                    "title": "qutils Notification!",
                    "category": notification.CATEGORY_STATUS,
                    "defaults": notification.DEFAULT_LIGHTS,
                    "flags": notification.FLAG_AUTO_CANCEL,
                    "priority": notification.PRIORITY_MAX,
                    "notificationTag": "instant_notification"
                };

                if (cbVibrate.checked) {
                    notif["defaults"] = notif["defaults"] | notification.DEFAULT_VIBRATE;
                }

                if (cbSound.checked) {
                    notif["defaults"] = notif["defaults"] | notification.DEFAULT_SOUND;
                }

                notificationClient.schedule(notif);
            }
        }

        Button {
            color: root.color
            text: "Schedule Notification for 5 Seconds Later"
            width: parent.width
            height: SH.dp(50)
            onClicked: {
                notification.delay = 5000;
                notification.text = "You are now experiencing 5 seconds later!";
                notification.title = "qutils Scheduled Notification";
                notification.notificationTag = "5_second_tag";
                notificationClient.scheduleNotification(notification);

                if (cbVibrate.checked) {
                    notification.defaults = notification.defaults | notification.DEFAULT_VIBRATE;
                }

                if (cbSound.checked) {
                    notification.defaults = notification.defaults | notification.DEFAULT_SOUND;
                }
            }
        }

        Button {
            color: root.color
            text: "Schedule Notification for 15 Seconds Later"
            width: parent.width
            height: SH.dp(50)
            onClicked: {
                notification.delay = 15000;
                notification.text = "You are now experiencing 15 seconds later!";
                notification.title = "qutils Scheduled Notification";
                notification.notificationTag = "15_second_tag";
                notificationClient.scheduleNotification(notification);

                if (cbVibrate.checked) {
                    notification.defaults = notification.DEFAULT_VIBRATE;
                }

                if (cbSound.checked) {
                    notification.defaults |= notification.DEFAULT_SOUND;
                }
            }
        }
    }

    CheckBox {
        id: cbVibrate
        anchors {
            top: cl.bottom
            topMargin: SH.dp(10)
            horizontalCenter: parent.horizontalCenter
        }
        font.pointSize: 13
        text: "Vibrate"
    }

    CheckBox {
        id: cbSound
        width: cbVibrate.width
        height: cbVibrate.height
        anchors {
            top: cbVibrate.bottom
            topMargin: SH.dp(10)
            horizontalCenter: parent.horizontalCenter
        }
        font.pointSize: 13
        text: "Sound"
    }
}
