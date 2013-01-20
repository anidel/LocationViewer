// Navigation pane project template
import bb.cascades 1.0

NavigationPane {
    id: navigationPane
    objectName: "navigationPane"
    peekEnabled: false
    
    Page {
        titleBar: TitleBar {
            title : "Location viewer"
        }
        
        // page with a picture thumbnail
        Container {
            background: Color.LightGray
            layout: DockLayout {
            }
            
            ListView {
                id: listView
                layout: GridListLayout {
                    verticalCellSpacing: 8
                    horizontalCellSpacing: 8
                    columnCount: 2
                }
                
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                
                dataModel: listModel.model

                property Page secondPage
                
                onTriggered: {
                    clearSelection()
                    select(indexPath)
                }
                
                onSelectionChanged: {
//                    var page = getSecondPage();                    
//                    navigationPane.push(page);
                    listModel.selectionChanged(indexPath, selected)
                }
                
                function getSecondPage() {
                    if (! secondPage) {
                        secondPage = secondPageDefinition.createObject();
                    }
                    return secondPage;
                }
                
                attachedObjects: [
                    ComponentDefinition {
                        id: secondPageDefinition
                        
                        source: "mapViewPage.qml"
                    },                    
                    // This handler is tracking the scroll state of the ListView.
                    ListScrollStateHandler {
                        id: scrollStateHandler
                        
                        onScrollingChanged: {
                            if (!scrolling) {
//                                listModel 
                            }
                        }
                    }
                ]
                
                listItemComponents: ListItemComponent {
                    type: ""
                    
                    // The ImageView that shows the loaded image after loading has finished without error
                    Container {
                        layout: DockLayout {}

                        
                        // The ActivityIndicator that is only active and visible while the image is loading
                        ActivityIndicator {
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Center
                            preferredHeight: 300
                    
                            visible: ListItemData.loading
                            running: ListItemData.loading
                        }
                        
                        ImageView {
                            id: image
                            
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Fill
                            scalingMethod: ScalingMethod.AspectFill                       
                            
                            image: ListItemData.image
                            visible: !ListItemData.loading && ListItemData.label == ""
                            
                            onCreationCompleted: {
                                ListItemData.load();
                            }
                        }
                        
                        Label {
                            verticalAlignment: VerticalAlignment.Bottom
                            horizontalAlignment: HorizontalAlignment.Center
                            
                            visible: !ListItemData.loading && !ListItemData.label == ""
                            text: ListItemData.label
                            multiline: true                        
                                                        
                            textStyle {
                                fontSize: FontSize.XXSmall
                                color: Color.Green
                            }
                        }
                    }
                }                       
            }
        }
    }

    onCreationCompleted: {
        // this slot is called when declarative scene is created
        // write post creation initialization here
        console.log("NavigationPane - onCreationCompleted()");

        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
    }
    
//    onPopTransitionEnded: { page.destroy(); }
}
