import QtQuick 2.7
import "../"

Item {
    property var colorSet: [
        "#000000",
        "#666666",
        "#a6a6a6",
        "#e3e3e3",
        "#ffffff"
    ]
    property var tickColorSet: [
            "#2cdd45",
            "#56e46a",
            "#95eea2",
            "#d5f8da",
            "#ffffff"
        ]

    property var background: {
        "normal": colorSet[2],
        "checked": colorSet[3],
        "normal_disabled": colorSet[3],
        "checked_disabled": colorSet[3]
    }

    property var text: {
        "normal": colorSet[1],
        "checked": colorSet[1],
        "normal_disabled": colorSet[2],
        "checked_disabled": colorSet[2]
    }

    property var tick: {
        "checked": tickColorSet[1],
        "checked_disabled": tickColorSet[2],
        "hovered": "transparent"
    }
}
