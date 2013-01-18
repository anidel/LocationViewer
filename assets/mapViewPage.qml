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
        
        MapView
        {
            id: mapView
            objectName: "mapView"
            
            preferredWidth: 720
            preferredHeight: 400
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom

            altitude: 3000
            latitude: 51.4818
            longitude: -0.2905
        }
        
        // The ListView that shows the progress of loading and result images
        ListView
        {
            id: listViewMapMode
            layout: StackListLayout
            {
                orientation: LayoutOrientation.LeftToRight
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
                        mapView.latitude = listModel.latitude;
                        mapView.longitude = listModel.longitude;
                    }
                }
            }
        ]
            listItemComponents: ListItemComponent
            {
                Container
                {
                    background: Color.White
                    preferredWidth: 768
                    horizontalAlignment: HorizontalAlignment.Center
                    
                    layout: DockLayout
                    {}
                    
//                    onTouch: {
//                        ListItem.view.currentIndexInSection = ListItem.indexInSection;
//                        ListItem.view.sectionSize = ListItem.sectionSize
//                        ListItem.view.map.latitude = ListItemData.latitude;
//                        ListItem.view.map.longitude = ListItemData.longitude;
//                        ListItem.view.map.altitude = ListItemData.altitude;
//                    }
                    
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
                        
                        text: "Image no: "+ListItemData.idx + "\n"+ListItemData.title;
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
}