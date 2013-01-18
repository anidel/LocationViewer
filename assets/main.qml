// Navigation pane project template
import bb.cascades 1.0

NavigationPane {
    id: navigationPane
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
                    var page = getSecondPage();
                    navigationPane.push(page);
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
                    }
                ]
                
                listItemComponents: ListItemComponent {
                    type: ""
                    // The ImageView that shows the loaded image after loading has finished without error
                    ImageView {
                        id: image
                        
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                        scalingMethod: ScalingMethod.AspectFill                       
                        
                        imageSource: ListItemData.imageSource
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
}
