import bb.cascades 1.0
import QtMobility.sensors 1.2
import bb.cascades.maps 1.0
import QtMobilitySubset.location 1.1

Page
{
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
//            title: "Previous page"
//            imageSource: "asset:///back.png"
            onTriggered: { navigationPane.pop(); }
        }
    }
   
    Container
    {
        id: listViewContainer
        background: Color.DarkGray
        
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
                    var xy = listModel.worldToPixelInvokable(mapView, marker.lat, marker.lon);
                    marker.x = xy.split(" ")[0];
                    marker.y = xy.split(" ")[1];
                    pinContainer.add(marker);
                    marker.animDrop.play();
                }
                function updateMarkers() {
                    listModel.updateMarkers(mapView, pinContainer);
                }
            }
        }
        
        // The ListView that shows the progress of loading and result images
        ListView
        {
            id: listViewMapMode
            layoutProperties: AbsoluteLayoutProperties 
            {
                positionY: 408
            }
            
            layout: StackListLayout
            {
                orientation: LayoutOrientation.TopToBottom
            }
            
            property int currentIndexInSection
            property int sectionSize
            property int leftToRight
            property double touchDownX
            property double moveThreshold: 100
            property MapView mapViewElement: mapView

            flickMode: ListView.SingleItem;
            snapMode: ListView.LeadingEdge
            
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            dataModel: listModel.model

            attachedObjects: [
                // This handler is tracking the scroll state of the ListView.
                ListScrollStateHandler {
                id: scrollStateHandler
 
                onScrollingChanged: {
                    if (!scrolling) {
                        listModel.map (firstVisibleItem);
                        if (listModel.latitude > -999999.99)
                        {
                            mapView.latitude = listModel.latitude;
                            mapView.longitude = listModel.longitude;
                            pinContainer.removeAll ();
                            pinContainer.addPin(listModel.latitude,listModel.longitude)
                        } else {
                            mapView.opacity = 0.5;
                        }
                    }
                }
            }
        ]
            listItemComponents: ListItemComponent
            {
                Container
                {
                    background: Color.White
                    preferredHeight: 690

                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    
                    layout: DockLayout
                    {}

                    // The ImageView that shows the loaded image after loading has finished without error
                    ImageView
                    {
                        id: imageMapView
                        
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                        scalingMethod: ScalingMethod.AspectFill
                            
                        imageSource: ListItemData.imageSource                        
                    }

                    Label {
                        id: imageMapViewLabel
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Bottom
                        
                        text: "Camera model: "+ListItemData.cameraModel + "\nDate taken: "+ListItemData.dateTaken;
                        multiline: true;
                        textStyle {
                            color: Color.Green
                            fontSize: FontSize.XSmall
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