import bb.cascades 1.0
import QtMobility.sensors 1.2
import bb.cascades.maps 1.0
import QtMobilitySubset.location 1.1
import anidel.library 1.0

Page
{
    id: mapViewPage
    objectName: "mapViewPage"
    
    actions: [
        ActionItem {
            id: centerOnPinAction

            title: "Center map"
            imageSource: "asset:///images/pin.png"
            onTriggered: {
                mapView.latitude = mapViewListModel.latitude;
                mapView.longitude = mapViewListModel.longitude;
                mapView.altitude = 3000;
                
                pinContainer.updateMarkers();
            }
            
            ActionBar.placement: ActionBarPlacement.OnBar
        }
    ]
    
    Container
    {
        id: pageContainer
        background: Color.LightGray
        layout: StackLayout{}
        
        Container {
            id: shadowContainer
            layout: DockLayout {}
            preferredWidth: 720
            preferredHeight: 510
            horizontalAlignment: HorizontalAlignment.Center
            ImageView
            {
                topMargin: 2
                leftMargin: 2
                imageSource: "asset:///images/shadow.amd"
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
            }
            
            Container {
                id: mapViewContainer
                layout: DockLayout {}
                preferredWidth: 696
                preferredHeight: 486
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center            

                MapView
                {
                    id: mapView
                    objectName: "mapView"
                    altitude: 3000
                    latitude: 51.4818
                    longitude: -0.2905
                    onTouch: {
                        pinContainer.updateMarkers();
                    }
                }
                
                Container {
                id: pinContainer
                preferredHeight: 450
                layout: AbsoluteLayout {}
                overlapTouchPolicy: OverlapTouchPolicy.Allow
                function addPin(lat, lon) {
                    var marker = pin.createObject();
                    marker.lat = lat;
                    marker.lon = lon;
                    var xy = mapViewListModel.worldToPixelInvokable(mapView, marker.lat, marker.lon);
                    marker.x = xy.split(" ")[0];
                    marker.y = xy.split(" ")[1];
                    pinContainer.add(marker);
                    marker.animDrop.play();
                }
                function updateMarkers() {
                    mapViewListModel.updateMarkers(mapView, pinContainer);
                }
            }
            Container {
                id: noLocationLabelContainer
                visible: false
                background: Color.Black
                opacity: 0.8
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                preferredWidth: 696;
                
                Label {
                    id: noLocationLabel
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    
                    text: "No location stored inside the picture!"
                    textStyle {
                        color: Color.Red
                    }
                }
            }
        }
    }
        
        Container {
            background: Color.Transparent
            preferredWidth: 696
//            preferredHeight: 640
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Center
            layout: DockLayout {}
            // The ListView that shows the progress of loading and result images
            ListView
            {
                id: listViewMapMode
                objectName: "listViewMapMode"
                onCreationCompleted: {
                    timer.start();
                }
                layout: StackListLayout { orientation: LayoutOrientation.TopToBottom }
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                flickMode: ListView.SingleItem;
                snapMode: ListView.LeadingEdge                        
                dataModel: mapViewListModel.model
                attachedObjects: [
                    QTimer{
                        id: timer
                        singleShot: true
                        interval: 10000
                        onTimeout:{
                            pinContainer.updateMarkers();
                        }
                    },
                    // This handler is tracking the scroll state of the ListView.
                    ListScrollStateHandler {
                        id: scrollStateHandler
                        onFirstVisibleItemChanged: {                        
                            if (firstVisibleItem >= 0) {
                                mapViewListModel.map (firstVisibleItem);
                                if (mapViewListModel.latitude > -9999.99) {
                                    mapView.visible = true;
                                    noLocationLabelContainer.visible = false;
                                    centerOnPinAction.enabled = true;
                                    mapView.latitude = mapViewListModel.latitude;
                                    mapView.longitude = mapViewListModel.longitude;
                                    pinContainer.removeAll ();
                                    pinContainer.addPin(mapViewListModel.latitude,mapViewListModel.longitude)
                                } else {
                                    noLocationLabelContainer.visible = true;
                                    centerOnPinAction.enabled = false;
                                    pinContainer.removeAll ();
                                }
                            }
                        }
                    }
                ]
                listItemComponents: ListItemComponent
                {
                    type: ""
                    Container
                    {
//                        preferredHeight: 640
                        preferredWidth: 696
                        verticalAlignment: VerticalAlignment.Fill
                        horizontalAlignment: HorizontalAlignment.Fill
                        layout: DockLayout {}
                        // The ActivityIndicator that is only active and visible while the image is loading
                        ActivityIndicator {
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Center
                            preferredHeight: 300
                            
                            visible: ListItemData.loading
                            running: ListItemData.loading
                        }
                        
                        // The ImageView that shows the loaded image after loading has finished without error
                        ImageView
                        {
                            id: imageMapView
                            scalingMethod: ScalingMethod.AspectFit
                            image: ListItemData.image
                            visible: !ListItemData.loading && ListItemData.label == ""
                            preferredHeight: 630
                            maxHeight: 630
                            horizontalAlignment: HorizontalAlignment.Center
                            // This custom property stores the initial scale of the image when a
                            // pinch gesture begins
                            property double initialScale: 1.0
                            // This custom property determines how quickly the image grows or
                            // shrinks in response to the pinch gesture
                            property double scaleFactor: 0.8
                            onCreationCompleted: {
                                ListItemData.load();
                            }
                            gestureHandlers: [
                                DoubleTapHandler {
                                    onDoubleTapped: {
                                        if (imageMapView.scaleX == 1.0) {
                                            imageMapView.scaleX = 1.5
                                            imageMapView.scaleY = 1.5
                                        } else {
                                            imageMapView.scaleX = 1.0
                                            imageMapView.scaleY = 1.0
                                        }
                                    }
                                },
                                // Add a handler for pinch gestures
                                PinchHandler {
                                    // When the pinch gesture starts, save the initial scale of
                                    // the image
                                    onPinchStarted: {
                                        imageMapView.initialScale = imageMapView.scaleX;
                                    }
                                    // As the pinch expands or contracts, change the scale of the
                                    // image
                                    onPinchUpdated: {
                                        imageMapView.scaleX = imageMapView.initialScale + ((event.pinchRatio - 1) * imageMapView.scaleFactor);
                                        imageMapView.scaleY = imageMapView.initialScale + ((event.pinchRatio - 1) * imageMapView.scaleFactor); 
                                    }
                                    onPinchEnded: {
                                        if (imageMapView.scaleX < 1.0) {
                                            imageMapView.scaleX = 1.0
                                            imageMapView.scaleY = 1.0
                                        }
                                        if (imageMapView.scaleX > 3.0) {
                                            imageMapView.scaleX = 3.0
                                            imageMapView.scaleY = 3.0
                                        }
                                    }
                                }
                            ]
                        }
                        Container
                        {
                            background: Color.Black
                            opacity: 0.5
                            preferredWidth: 720
                            verticalAlignment: VerticalAlignment.Bottom
                            horizontalAlignment: HorizontalAlignment.Center
                            overlapTouchPolicy: OverlapTouchPolicy.Allow
                            
                            Label
                            {
                                id: imageMapViewLabel
                                opacity: 1
                                horizontalAlignment: HorizontalAlignment.Center
                                verticalAlignment: VerticalAlignment.Bottom
                                text: ListItemData.cameraModel + "\n"+ListItemData.dateTaken;
                                multiline: true;
                                textStyle {
                                    color: Color.White
                                    fontSize: FontSize.Small
                                }
                            }
                        }                    
                    }
                }
            }
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: pin
            source: "pin.qml"
        }
    ]
}