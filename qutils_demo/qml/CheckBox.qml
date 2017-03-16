import QtQuick 2.7

Item {
    property var checkBoxStyle: CheckBoxStyle {}

    property alias checkBoxSize: checkSign.checkBoxSize
    property int _shortLineWidth: Math.sqrt(Math.pow(checkBoxSize * 0.4, 2) * 2)
    property int _longLineWidth: Math.sqrt(Math.pow(checkBoxSize * 0.9, 2) * 2)
    property int lineThickness: SH.dp(5)

    property bool checked: false
    property alias text: lbText.text
    property alias font: lbText.font

    property int textLeftMargin: checkSign.width * 0.8
    readonly property size fixedSize: Qt.size(checkSign.width + lbText.contentWidth + lbText.anchors.leftMargin, Math.max(checkSign.height, lbText.contentHeight));

    signal hovered(var hovering)
    signal pressed()
    signal cancelled()
    signal clicked()
    signal linkActivated(var link)

    id: root
    width: checkSign.width + lbText.contentWidth + lbText.anchors.leftMargin
    height: Math.max(checkSign.height, lbText.contentHeight)
    state: "unchcked"
    states: [
        State {
            name: "checked"
            when: checked

            PropertyChanges { target: checkSign; color: checkBoxStyle.background.checked }
            PropertyChanges { target: lbText; color: checkBoxStyle.text.checked }
        },
        State {
            name: "normal"
            when: !checked

            PropertyChanges { target: checkSign; color: checkBoxStyle.background.normal }
            PropertyChanges { target: lbText; color: checkBoxStyle.text.normal }
        },
        State {
            name: "chcked_disabled"

            PropertyChanges { target: checkSign; color: checkBoxStyle.background.checked_disabled }
            PropertyChanges { target: lbText; color: checkBoxStyle.text.checked_disabled }
        },
        State {
            name: "unchcked_disabled"

            PropertyChanges { target: checkSign; color: checkBoxStyle.background.normal_disabled }
            PropertyChanges { target: lbText; color: checkBoxStyle.text.normal_disabled }
        }
    ]
    transitions: [
        Transition {
            from: "normal"
            to: "checked"

            ColorAnimation { duration: 300 }
        },
        Transition {
            from: "checked"
            to: "normal"

            ColorAnimation { duration: 300 }
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
    onStateChanged: checkSign.state = root.state

    CheckSign {
        id: checkSign
        width: checkBoxSize
        height: checkBoxSize
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }
        interactive: true
        checkBoxSize: SH.dp(18)
        checkBoxStyle: root.checkBoxStyle
    }

    Text {
        id: lbText
        anchors {
            left: checkSign.right
            leftMargin: root.textLeftMargin
            verticalCenter: parent.verticalCenter
        }
        verticalAlignment: Text.AlignVCenter
        onLinkActivated: root.linkActivated(link)
    }

    MouseArea {
        property bool hovering: false

        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            checked = !checked;
            checkSign.checked = checked;
            root.clicked();
        }
        onEntered: {
            hovering = true;
            root.hovered(hovering);
        }
        onExited: {
            hovering = false;
            root.hovered(hovering);
        }
        onPressed: root.pressed()
        onCanceled: root.cancelled()
    }
}
