import QtQuick 2.7

Item {
    // ----- Public Properties ----- //

    property size blockSize: Qt.size(SH.dp(50), SH.dp(50))
    property int spacing: SH.dp(10)
    readonly property int colorCount: gvColors.model.count

    // ----- Signals ----- //

    signal clicked(var color)

    id: root
    height: gvColors.contentHeight + lbTitle.height

    Text {
        id: lbTitle
        text: "StatusBar Colors"
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        font.pointSize: 15
        color: "#EF5350"
    }

    GridView {
        id: gvColors
        width: parent.width
        height: contentHeight
        anchors {
            top: lbTitle.bottom
            topMargin: lbTitle.height
            horizontalCenter: parent.horizontalCenter
        }
        clip: true
        cellWidth: blockSize.width + spacing
        cellHeight: blockSize.width + spacing
        model: ListModel {
            ListElement { cell_color: "#F44336" }
            ListElement { cell_color: "#E91E63" }
            ListElement { cell_color: "#9C27B0" }
            ListElement { cell_color: "#F8BBD0" }
            ListElement { cell_color: "#E1BEE7" }
            ListElement { cell_color: "#64B5F6" }

            ListElement { cell_color: "#673AB7" }
            ListElement { cell_color: "#8C9EFF" }
            ListElement { cell_color: "#2979FF" }
            ListElement { cell_color: "#009688" }
            ListElement { cell_color: "#80DEEA" }
            ListElement { cell_color: "#CDDC39" }
        }
        delegate: Component {
            Rectangle {
                width: root.blockSize.width
                height: root.blockSize.height
                color: cell_color
                radius: SH.dp(7)

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        root.clicked(cell_color);
                    }
                }
            }
        }
    }
}
