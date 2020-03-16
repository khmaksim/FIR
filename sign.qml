import QtQuick 2.0
import QtLocation 5.9

MapQuickItem {
    anchorPoint.x: image.width/4
    anchorPoint.y: image.height
    property alias heightObstracle: labelHeight.text
    property int type: 0
    property alias idObstracle: labelId.text
    property bool selected: false

    sourceItem: Column {
        Row {
            Image {
                property var srcImgSign: [
                    "qrc:/images/res/img/N.png",
                    "qrc:/images/res/img/A.png",
                    "qrc:/images/res/img/M.png",
                    "qrc:/images/res/img/G.png",
                    "qrc:/images/res/img/KTA.png"
                ]
                id: image
                source: srcImgSign[type]
                width: type == 0 ? 5 : 20
                height: type == 0 ? 5 : 30
            }
            Text {
                id: labelHeight
            }
        }
        Text {
            id: labelId
            color: selected ? "red" : "black"
        }
    }
}
