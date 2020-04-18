import QtQuick          2.6
import QtQuick.Controls 1.4
import QtLocation       5.13
import QtPositioning    5.13

MapQuickItem {
    id: _endcurve
    anchorPoint.x: x1
    anchorPoint.y: y1
    property real _width: 24
    property real _height: 24
    property real x1: 0.0
    property real x2: 0.0
    property real y1: 0.0
    property real y2: 0.0
    property real ang: 0

    function callPaint(){
        _canvas.requestPaint()
    }

    sourceItem: Rectangle {
        id: _wayPointImage
        color: "transparent"
        property color strokeStyle:  "red"
        width: _width
        height: _height
        Canvas {
            id: _canvas
            anchors.fill: parent
            antialiasing: true
            onPaint: {
                console.log("callPaint")
                var ctx = getContext("2d");
                ctx.beginPath()
                ctx.strokeStyle = _wayPointImage.strokeStyle;
                ctx.moveTo(x1, y1)
                ctx.lineWidth  = 4
                ctx.bezierCurveTo(x2, y2)
                ctx.closePath();
                ctx.stroke();
                //                 ctx.rotate(ang);
            }
        }
    }
}
