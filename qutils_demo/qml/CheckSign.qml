import QtQuick 2.7

Rectangle {
    property var checkBoxStyle

    property int checkBoxSize: root.height
    property int _shortLineWidth: Math.sqrt(Math.pow(checkBoxSize * 0.4, 2) * 2)
    property int _longLineWidth: Math.sqrt(Math.pow(checkBoxSize * 0.8, 2) * 2)
    property int lineThickness: SH.dp(5)

    property bool checked: false
    property bool interactive: true

    signal hovered(var hovering)
    signal clicked()

    id: root
    state: "unchcked"
    states: [
        State {
            name: "checked"
            when: checked

            PropertyChanges { target: partOne; width: _shortLineWidth }
            PropertyChanges { target: partTwo; width: _longLineWidth }
            PropertyChanges { target: root; color: root.interactive ? checkBoxStyle.background.checked : root.color }
            PropertyChanges { target: checkSilhouette; opacity: 0 }
        },
        State {
            name: "normal"
            when: !checked

            PropertyChanges { target: partOne; width: 0 }
            PropertyChanges { target: partTwo; width: 0 }
            PropertyChanges { target: root; color: checkBoxStyle.background.normal }
            PropertyChanges { target: checkSilhouette; opacity: (mouseArea.hovering ? 1 : 0) }
        },
        State {
            name: "chcked_disabled"
            when: checked

            PropertyChanges { target: partOne; width: _shortLineWidth; color: checkBoxStyle.tick.checked_disabled }
            PropertyChanges { target: partTwo; width: _longLineWidth; color: checkBoxStyle.tick.checked_disabled }
            PropertyChanges { target: root; color: checkBoxStyle.background.checked_disabled }
        },
        State {
            name: "unchcked_disabled"
            when: !checked

            PropertyChanges { target: partOne; width: 0 }
            PropertyChanges { target: partTwo; width: 0 }
            PropertyChanges { target: root; color: checkBoxStyle.background.normal_disabled }
        }
    ]
    transitions: [
        Transition {
            from: "normal"
            to: "checked"

            ColorAnimation { duration: 300 }
            SequentialAnimation {
                NumberAnimation { target: partOne; property: "width"; duration: 150; easing.type: Easing.InOutQuad; from: 0; to: _shortLineWidth }
                PauseAnimation { duration: 10 }
                NumberAnimation { target: partTwo; property: "width"; duration: 150; easing.type: Easing.OutBack; from: 0; to: _longLineWidth }
            }
        },
        Transition {
            from: "checked"
            to: "normal"

            ColorAnimation { duration: 300 }
            SequentialAnimation {
                NumberAnimation { target: partTwo; property: "width"; duration: 150; easing.type: Easing.InOutQuad; from: _longLineWidth; to: 0 }
                PauseAnimation { duration: 10 }
                NumberAnimation { target: partOne; property: "width"; duration: 150; easing.type: Easing.InOutQuad; from: _shortLineWidth; to: 0 }
            }
        },
        Transition {
            from: "checked"
            to: "checked_disabled"

            ColorAnimation { duration: 300 }
        },
        Transition {
            from: "checked_disabled"
            to: "checked"

            ColorAnimation { duration: 300 }
        }
    ]
    onCheckedChanged: root.state = checked ? "checked" : "normal"

    Item {
        id: checkContainer
        width: root.checkBoxSize
        height: width
        anchors {
            left: root.interactive ? parent.left : parent.horizontalCenter
            leftMargin: root.interactive ? 0 : checkContainer.width / -2
            verticalCenter: parent.verticalCenter
        }

        Rectangle {
            id: partOne
            width: 0
            height: lineThickness
            radius: SH.dp(25)
            y: root.checkBoxSize * 0.7 - lineThickness
            color: checkBoxStyle.tick.checked
            transform: Rotation {
                origin.x: 0
                origin.y: partOne.height / 2
                angle: 45
            }

            Behavior on width { NumberAnimation { duration: 300; easing.type: Easing.InExpo } }
        }

        Rectangle {
            id: partTwo
            width: 0
            height: lineThickness
            radius: partOne.radius
            x: root.checkBoxSize * 0.4 - lineThickness * 0.5
            y: checkContainer.height - lineThickness / 2
            color: checkBoxStyle.tick.checked
            transform: Rotation {
                origin.x: 0
                origin.y: partOne.height / 2
                angle: -45
            }

            Behavior on width { NumberAnimation { duration: 300; easing.type: Easing.OutBack } }
        }
    }

    Item {
        id: checkSilhouette
        width: root.checkBoxSize
        height: width
        anchors {
            left: root.interactive ? parent.left : parent.horizontalCenter
            leftMargin: root.interactive ? 0 : checkContainer.width / -2
            verticalCenter: parent.verticalCenter
        }

        Rectangle {
            id: partOneSilhouette
            width: _shortLineWidth
            height: lineThickness
            radius: SH.dp(25)
            y: root.checkBoxSize * 0.7 - lineThickness
            color: checkBoxStyle.tick.checked
            transform: Rotation {
                origin.x: 0
                origin.y: partOne.height / 2
                angle: 45
            }
        }

        Rectangle {
            id: partTwoSilhouette
            width: _longLineWidth
            height: lineThickness
            radius: partOne.radius
            x: root.checkBoxSize * 0.4 - lineThickness * 0.5
            y: checkContainer.height - lineThickness / 2
            color: checkBoxStyle.tick.checked
            transform: Rotation {
                origin.x: 0
                origin.y: partOne.height / 2
                angle: -45
            }
        }
    }

    MouseArea {
        property bool hovering: false

        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            if (interactive) {
                checked = !checked;
            }
            root.clicked();
        }
        onEntered: {
            if (interactive) {
                if (root.state !== "checked") {
                    checkSilhouette.opacity = 1;
                }
            }
            hovering = true;
            root.hovered(hovering);
        }
        onExited: {
            if (interactive) {
                if (root.state !== "checked") {
                    checkSilhouette.opacity = 0;
                }
            }

            hovering = false;
            root.hovered(hovering);
        }
    }
}
