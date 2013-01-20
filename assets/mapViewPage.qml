import bb.cascades 1.0
import QtMobility.sensors 1.2
import bb.cascades.maps 1.0
import QtMobilitySubset.location 1.1

Page
{
    id: mapViewPage
    objectName: "mapViewPage"
        
    actions: [
        ActionItem {
            id: centerOnPinAction

            title: "Center on pin"
            imageSource: "asset:///images/pin.png"
            onTriggered: {
                mapView.latitude = mapViewListModel.latitude;
                mapView.longitude = mapViewListModel.longitude;
                mapView.altitude = 3000;
            }
            
            ActionBar.placement: ActionBarPlacement.OnBar
        }
    ]
    
    onCreationCompleted: {
        mapViewListModel.linkMapData (mapView);
    }

    Container
    {
        id: listViewContainer
        background: Color.Black
        
        layout: StackLayout
        {
        }
        
        Container {
            layout: DockLayout {}
            MapView
            {
                id: mapView
                objectName: "mapView"
                            
                layoutProperties: StackLayoutProperties {}
                preferredHeight: 450
                            
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Bottom
                            
                altitude: 3000
                latitude: 51.4818
                longitude: -0.2905
                
                onRequestRender: {
                    pinContainer.updateMarkers();
                }
            }

            Container {
                id: pinContainer
                // Must match the mapview width and height and position
                preferredHeight: 450
                //touchPropagationMode: TouchPropagationMode.PassThrough
                overlapTouchPolicy: OverlapTouchPolicy.Allow
                property variant me
                layout: AbsoluteLayout {
                }
                function addPin(lat, lon) {
                    var marker = pin.createObject();
                    marker.lat = lat;
                    marker.lon = lon;
//                    var xy = mapViewListModel.worldToPixelInvokable(mapView, marker.lat, marker.lon);
//                    marker.x = xy.split(" ")[0];
//                    marker.y = xy.split(" ")[1];
                    
                    // We center the map on lat/lon, so we can put the pin right at the center
                    marker.x = 384;
                    marker.y = 225;
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
                preferredWidth: 720;
                preferredHeight: 450;
                
                Label {
                    id: noLocationLabel
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 100
                    
                    text: "No location stored inside the picture!"
                    textStyle {
                        color: Color.Red
                    }
                }
            }
        }
        
        // The ListView that shows the progress of loading and result images
        ListView
        {
            id: listViewMapMode
            objectName: "listViewMapMode"
            
            onCreationCompleted: {
                pinContainer.addPin (ListItemData.latitude, ListItemData.longitude);
            }
            
            layout: StackListLayout
            {
                orientation: LayoutOrientation.TopToBottom
            }
            
            flickMode: ListView.SingleItem;
            snapMode: ListView.LeadingEdge
            
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
                        
            dataModel: mapViewListModel.model
            
            attachedObjects: [
                // This handler is tracking the scroll state of the ListView.
                ListScrollStateHandler {
                    id: scrollStateHandler
                    
                    onFirstVisibleItemChanged: {
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
//                    onScrollingChanged: {
//                        if (!scrolling) {
//                            
//                            listModel.map (firstVisibleItem);
//                            if (listModel.latitude > -9999.99) {
//                                mapView.visible = true;
//                                noLocationLabel.visible = false;
//                                
//                                mapView.latitude = listModel.latitude;
//                                mapView.longitude = listModel.longitude;
//                                pinContainer.removeAll ();
//                                pinContainer.addPin(listModel.latitude,listModel.longitude)
//                            } else {
//                                mapView.visible = false;
//                                noLocationLabel.visible = true;
//                                pinContainer.removeAll ();
//                            }
//                        }
//                    }
                }
            ]
            
            listItemComponents: ListItemComponent
            {
                Container
                {
                    background: Color.Black
                    preferredHeight: 690

                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    
                    layout: DockLayout
                    {}
                    
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
                        
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                        scalingMethod: ScalingMethod.AspectFit
                            
                        image: ListItemData.image
                        // This custom property stores the initial scale of the image when a
                        // pinch gesture begins
                        property double initialScale: 1.0
                        
                        // This custom property determines how quickly the image grows or
                        // shrinks in response to the pinch gesture
                        property double scaleFactor: 0.8
                        
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
                                    imageMapView.scaleX = imageMapView.initialScale +
                                        ((event.pinchRatio - 1) * imageMapView.scaleFactor);
                                    imageMapView.scaleY = imageMapView.initialScale +
                                        ((event.pinchRatio - 1) * imageMapView.scaleFactor); 
                                }
                                
                                onPinchEnded: {
                                    if (imageMapView.scaleX < 1.0) {
                                        imageMapView.scaleX = 1.0
                                        imageMapView.scaleY = 1.0
                                    }
                                }
                            }
                        ]
                    }
                    
                    Container {
                        background: Color.Black
                        opacity: 0.5
                        verticalAlignment: VerticalAlignment.Bottom
                        horizontalAlignment: HorizontalAlignment.Center
                        preferredWidth: 720;
                        
                        Label {
                            id: imageMapViewLabel
                            opacity: 1
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Bottom
                            
                            text: ListItemData.cameraModel + "\n"+ListItemData.dateTaken;
                            multiline: true;
                            textStyle {
                                color: Color.White
                                fontSize: FontSize.Medium
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