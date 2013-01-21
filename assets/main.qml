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
                    verticalCellSpacing: 4
                    horizontalCellSpacing: 4
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
                    listModel.selectionChanged(indexPath, selected)
                }
                                
                listItemComponents: ListItemComponent {
                    type: ""

                    // The ImageView that shows the loaded image after loading has finished without error
                    Container {
                        layout: DockLayout {}
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                        
                        // The ActivityIndicator that is only active and visible while the image is loading
                        ActivityIndicator {
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Center
                            preferredHeight: 300
                    
                            visible: ListItemData.loading
                            running: ListItemData.loading
                        }
                        
                        Container {
                            layout: StackLayout {}
                            topPadding: 14
                            leftPadding: 14
                            bottomPadding: 16
                            rightPadding: 16
                            
                            ImageView {
                                id: image
                                
                                horizontalAlignment: HorizontalAlignment.Left
                                verticalAlignment: VerticalAlignment.Top
                                scalingMethod: ScalingMethod.AspectFill
                                
                                image: ListItemData.image
                                visible: !ListItemData.loading && ListItemData.label == ""
                                
                                onCreationCompleted: {
                                    ListItemData.load();
                                }
                                
                                animations: [
                                    SequentialAnimation {
                                        id: bounceAnimation
                                        ScaleTransition {
                                            id: animFocus
                                            fromX: 1
                                            toX: .8
                                            fromY: 1
                                            toY: .8
                                            duration: 150
                                        }
                                        ScaleTransition {
                                            id: animUnfocus
                                            fromX: .8
                                            toX: 1
                                            fromY: .8
                                            toY: 1
                                            duration: 150
                                        }
                                    }
                                ]
                            }
                        }
                                                    
                        ImageView {
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Fill
                            visible: !ListItemData.loading && ListItemData.label == ""
                                         
                            imageSource: "asset:///images/overlay.amd"
                        }
                        
                        ImageView {
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Fill
                            visible: !ListItemData.loading && ListItemData.label != ""
                            
                            imageSource: "asset:///images/picture1br.png"
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
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayPortrait;
    }
}
