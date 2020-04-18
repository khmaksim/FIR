import QtQuick 2.13
import QtQuick.Window 2.13
import QtLocation 5.13
import QtPositioning 5.13
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13

Item {
    id: root
    visible: true
    width: 640
    height: 480
    signal checked(bool f, string id);

    TabBar {
        id: toolBar
        width: parent.width
        TabButton {
            text: qsTr("ESRI")
        }
        TabButton {
            text: qsTr("OSM")
        }
        TabButton {
            text: qsTr("Mapbox")
        }
    }

    StackLayout {
        width: parent.width
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom

        currentIndex: toolBar.currentIndex
        Item {
            id: esriTab

            Map {
                id: mapEsriView
                anchors.fill: parent
                plugin: Plugin {
                    preferred: ["esri"]
//                    required: Plugin.AnyMappingFeatures | Plugin.AnyGeocodingFeatures
                }

                MouseArea {
                    anchors.fill: parent

//                    onDoubleClicked: {
//                        var coordinate = mapEsriView.toCoordinate(Qt.point(mouse.x,mouse.y))
//                        var numItems = mapEsriView.mapItems.length;

//                        for (var i = 0; i < numItems; i++) {
//                            if (mapEsriView.mapItems[i].objectName !== "circle") {
//                                var coordinateObstracle = mapEsriView.mapItems[i].coordinate;
//                                var d = 6371 * 2 * Math.asin(Math.sqrt(Math.pow(Math.sin(degreesToRadians((coordinate.latitude - coordinateObstracle.latitude) / 2)), 2) +
//                                                                       Math.cos(degreesToRadians(coordinateObstracle.latitude)) *
//                                                                       Math.cos(degreesToRadians(coordinate.latitude)) *
//                                                                       Math.pow(Math.sin(degreesToRadians(Math.abs(coordinate.longitude -
//                                                                                                                   coordinateObstracle.longitude) / 2)), 2)));
//                                if (d <= 0.05) {
//                                    mapEsriView.mapItems[i].selected = !mapEsriView.mapItems[i].selected;
//                                    root.checked(mapEsriView.mapItems[i].selected, mapEsriView.mapItems[i].idObstracle);
//                                    break;
//                                }
//                            }
//                        }
//                    }
                }
            }
        }
        Item {
            id: osmTab

            Map {
                id: mapOsmView
                anchors.fill: parent
                plugin: Plugin {
                    preferred: ["osm"]
//                    required: Plugin.AnyMappingFeatures | Plugin.AnyGeocodingFeatures
                }

                MouseArea {
                    anchors.fill: parent

//                    onDoubleClicked: {
//                        var coordinate = mapOsmView.toCoordinate(Qt.point(mouse.x,mouse.y))
//                        var numItems = mapOsmView.mapItems.length;

//                        for (var i = 0; i < numItems; i++) {
//                            if (mapOsmView.mapItems[i].objectName !== "circle") {
//                                var coordinateObstracle = mapOsmView.mapItems[i].coordinate;
//                                var d = 6371 * 2 * Math.asin(Math.sqrt(Math.pow(Math.sin(degreesToRadians((coordinate.latitude - coordinateObstracle.latitude) / 2)), 2) +
//                                                                       Math.cos(degreesToRadians(coordinateObstracle.latitude)) *
//                                                                       Math.cos(degreesToRadians(coordinate.latitude)) *
//                                                                       Math.pow(Math.sin(degreesToRadians(Math.abs(coordinate.longitude -
//                                                                                                                   coordinateObstracle.longitude) / 2)), 2)));
//                                if (d <= 0.05) {
//                                    mapOsmView.mapItems[i].selected = !mapOsmView.mapItems[i].selected;
//                                    root.checked(mapOsmView.mapItems[i].selected, mapOsmView.mapItems[i].idObstracle);
//                                    break;
//                                }
//                            }
//                        }
//                    }
                }
            }
        }
        Item {
            id: mapboxTab

            Map {
                id: mapMapboxView
                anchors.fill: parent
                plugin: Plugin {
                    name: "mapbox"
                    PluginParameter {
                        name: "mapbox.access_token"
                        value: "pk.eyJ1IjoibWF4aW1raCIsImEiOiJjazMzaTNoaTIwc2N6M25tajg4ZGhtbXdiIn0.KZ6632nxyVFDhN2i8QYVkw"
                    }
                }

//                MouseArea {
//                    anchors.fill: parent

//                    onDoubleClicked: {
//                        var coordinate = mapMapboxView.toCoordinate(Qt.point(mouse.x,mouse.y))
//                        var numItems = mapMapboxView.mapItems.length;

//                        for (var i = 0; i < numItems; i++) {
//                            if (mapMapboxView.mapItems[i].objectName !== "circle") {
//                                var coordinateObstracle = mapMapboxView.mapItems[i].coordinate;
//                                var d = 6371 * 2 * Math.asin(Math.sqrt(Math.pow(Math.sin(degreesToRadians((coordinate.latitude - coordinateObstracle.latitude) / 2)), 2) +
//                                                                       Math.cos(degreesToRadians(coordinateObstracle.latitude)) *
//                                                                       Math.cos(degreesToRadians(coordinate.latitude)) *
//                                                                       Math.pow(Math.sin(degreesToRadians(Math.abs(coordinate.longitude -
//                                                                                                                   coordinateObstracle.longitude) / 2)), 2)));
//                                if (d <= 0.05) {
//                                    mapMapboxView.mapItems[i].selected = !mapMapboxView.mapItems[i].selected;
//                                    root.checked(mapMapboxView.mapItems[i].selected, mapMapboxView.mapItems[i].idObstracle);
//                                    break;
//                                }
//                            }
//                        }
//                    }
//                }
            }
        }
    }

    Component {
        id: mapCircleComponent
        MapCircle {
            objectName: "circle"
            border.width: 1
            border.color: 'blue'
        }
    }

    function degreesToRadians(degrees) {
        return (degrees * Math.PI)/180;
    }

    function clearMap() {
        mapOsmView.clearMapItems();
        mapEsriView.clearMapItems();
        mapMapboxView.clearMapItems();
    }

    function setCenter(lat, lon) {
        mapOsmView.center = QtPositioning.coordinate(lat, lon);
        mapEsriView.center = QtPositioning.coordinate(lat, lon);
        mapMapboxView.center = QtPositioning.coordinate(lat, lon);
    }

    function getCenterOfPolygon(path){
        var x = 0;
        var y = 0;
        var z = 0;
        var lat1 = 0;
        var lon1 = 0;
        var numPoints = path.length;

        for (var i = 0; i < numPoints; i++) {
            var coordinate = QtPositioning.coordinate(path[i].x, path[i].y);
            lat1 = coordinate.latitude;
            lon1 = coordinate.longitude;

            lat1 = lat1 * Math.PI/180
            lon1 = lon1 * Math.PI/180
            x += Math.cos(lat1) * Math.cos(lon1)
            y += Math.cos(lat1) * Math.sin(lon1)
            z += Math.sin(lat1)
        }
        var lonCenter = Math.atan2(y, x)
        var Hyp = Math.sqrt(x * x + y * y)
        var latCenter = Math.atan2(z, Hyp)
        latCenter = latCenter * 180/Math.PI
        lonCenter = lonCenter * 180/Math.PI
        return QtPositioning.coordinate(latCenter, lonCenter);
    }

    function createPolyline(path, mapParent) {
//        var polyline = Qt.createQmlObject('import QtLocation 5.13; MapPolyline { line.width: 4; line.color: "#00D031"; }', mapParent)
        var numPoints = path.length;

        var endcurvesItem;
//        for (var i = 0; i < numPoints; i++)
//            polyline.addCoordinate(QtPositioning.coordinate(path[i].x, path[i].y));
//        for(var i = 0; i < numPoints - 1; i++) {
            var pt1 = Qt.point(10, 20);// mapParent.fromCoordinate(QtPositioning.coordinate(path[i].x, path[i].y), true);//gridCoordinatesList[i])
            var pt2 = Qt.point(30, 40);//mapParent.fromCoordinate(QtPositioning.coordinate(path[i+1].x, path[i+1].y), true);//gridCoordinatesList[i+1])

            console.log(pt1 + ' ' + pt2);

            var component = Qt.createComponent("CurveLine.qml");
            if (component.status === Component.Ready) {
                endcurvesItem = component.createObject(parent);
                endcurvesItem._width = Math.abs(pt2.x - pt1.x)
                endcurvesItem._height = Math.abs(pt2.y - pt1.y)
                endcurvesItem.x1 = pt1.x
                endcurvesItem.y1 = pt1.y
                endcurvesItem.x2 = pt2.x
                endcurvesItem.y2 = pt2.y
//                endcurvesItem.coordinate = QtPositioning.coordinate(path[i].x, path[i].y)

            }
            endcurvesItem.callPaint()
            mapParent.addMapItem(endcurvesItem)
//        }
//        mapParent.addMapItem(polyline)
    }

    function createLabel(coordinate, nameZone, codeIcao, nameSector, call, func, freq, mapParent) {
        var component = Qt.createComponent("qrc:/qml/label.qml");

        if (component.status === Component.Ready) {
            var label = component.createObject(parent);
            label.coordinate = coordinate;
            label.nameZone = nameZone
            label.codeIcao = codeIcao;
            label.nameSector = nameSector;
            label.call = call;
            label.func = func;
            label.freq = freq;
            mapParent.addMapItem(label);
        }
    }

    function displayZone(path, nameZone, codeIcao, nameSector, call, func, freq) {
        createPolyline(path, mapOsmView);
        createPolyline(path, mapEsriView);
        createPolyline(path, mapMapboxView);

        var coordinate = getCenterOfPolygon(path);
        setLabelOfZone(coordinate, nameZone, codeIcao, nameSector, call, func, freq);

//        polyline = Qt.createQmlObject('import QtLocation 5.13; MapPolyline { line.width: 3; line.color: "#FF4040"; }', mapEsriView)
////        var zoneBorder = Qt.createQmlObject('import QtLocation 5.13; MapPolyline { line.width: 1; line.color: "#42E712"; }', mapEsriView)
//        numPoints = path.length;
//        for (i = 0; i < numPoints; i++) {
//            polyline.addCoordinate(QtPositioning.coordinate(path[i].x, path[i].y));
////            zoneBorder.addCoordinate(QtPositioning.coordinate(path[i].x, path[i].y));
//        }
//        mapEsriView.addMapItem(polyline)
////        mapEsriView.addMapItem(zoneBorder)

    }

    function setLabelOfZone(coordinate, nameZone, codeIcao, nameSector, call, func, freq) {
        createLabel(coordinate, nameZone, codeIcao, nameSector, call, func, freq, mapOsmView);
        createLabel(coordinate, nameZone, codeIcao, nameSector, call, func, freq, mapEsriView);
        createLabel(coordinate, nameZone, codeIcao, nameSector, call, func, freq, mapMapboxView);
    }

//    function drawRadius(radius) {
//        var circle = mapCircleComponent.createObject(mapOsmView, {"center" : mapOsmView.center, "radius": radius * 1000});
//        mapOsmView.addMapItem(circle);
//        circle = mapCircleComponent.createObject(mapEsriView, {"center" : mapEsriView.center, "radius": radius * 1000});
//        mapEsriView.addMapItem(circle);
//        circle = mapCircleComponent.createObject(mapMapboxView, {"center" : mapMapboxView.center, "radius": radius * 1000});
//        mapMapboxView.addMapItem(circle);
//    }
}
