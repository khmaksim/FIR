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

    function addZone(path) {
        var zone = Qt.createQmlObject('import QtLocation 5.13; MapPolyline { line.width: 3; line.color: "#FF4040"; }', mapOsmView)
        var numPoints = path.length;
        for (var i = 0; i < numPoints; i++) {
            console.log(path[i]);
            console.log(path[i].x);
            console.log(path[i].y);
            zone.addCoordinate(QtPositioning.coordinate(path[i].x, path[i].y));
        }
        mapOsmView.addMapItem(zone)

        zone = Qt.createQmlObject('import QtLocation 5.13; MapPolyline { line.width: 3; line.color: "#FF4040"; }', mapEsriView)
        numPoints = path.length;
        for (i = 0; i < numPoints; i++) {
            console.log(path[i]);
            console.log(path[i].x);
            console.log(path[i].y);
            zone.addCoordinate(QtPositioning.coordinate(path[i].x, path[i].y));
        }
        mapEsriView.addMapItem(zone)

        zone = Qt.createQmlObject('import QtLocation 5.13; MapPolyline { line.width: 3; line.color: "#FF4040"; }', mapMapboxView)
        numPoints = path.length;
        for (i = 0; i < numPoints; i++) {
            console.log(path[i]);
            console.log(path[i].x);
            console.log(path[i].y);
            zone.addCoordinate(QtPositioning.coordinate(path[i].x, path[i].y));
        }
        mapMapboxView.addMapItem(zone)
    }

    function addMarker(lat, lon, height, type, id) {
        var component = Qt.createComponent("qrc:/qml/sign.qml");
        if (component.status === Component.Ready) {
            var sign = component.createObject(parent);
            sign.coordinate = QtPositioning.coordinate(lat, lon);
            sign.heightObstracle = height;
            sign.type = type;
            sign.idObstracle = id;
            mapEsriView.addMapItem(sign);
        }
        component = Qt.createComponent("qrc:/qml/sign.qml");
        if (component.status === Component.Ready) {
            sign = component.createObject(parent);
            sign.coordinate = QtPositioning.coordinate(lat, lon);
            sign.heightObstracle = height;
            sign.type = type;
            sign.idObstracle = id;
            mapOsmView.addMapItem(sign);
        }
        component = Qt.createComponent("qrc:/qml/sign.qml");
        if (component.status === Component.Ready) {
            sign = component.createObject(parent);
            sign.coordinate = QtPositioning.coordinate(lat, lon);
            sign.heightObstracle = height;
            sign.type = type;
            sign.idObstracle = id;
            mapMapboxView.addMapItem(sign);
        }
    }

    function drawRadius(radius) {
        var circle = mapCircleComponent.createObject(mapOsmView, {"center" : mapOsmView.center, "radius": radius * 1000});
        mapOsmView.addMapItem(circle);
        circle = mapCircleComponent.createObject(mapEsriView, {"center" : mapEsriView.center, "radius": radius * 1000});
        mapEsriView.addMapItem(circle);
        circle = mapCircleComponent.createObject(mapMapboxView, {"center" : mapMapboxView.center, "radius": radius * 1000});
        mapMapboxView.addMapItem(circle);
    }
}
