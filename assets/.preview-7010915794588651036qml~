// Navigation pane project template
import bb.cascades 1.0

NavigationPane {
    id: navigationPane
    Page {
        // page with a picture thumbnail
        Container {
            background: Color.Black
            layout: DockLayout {
            }
            Button {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                text: qsTr("Show detail")
                imageSource: "asset:///images/picture1thumb.png"
                onClicked: {
                    // show detail page when the button is clicked
                    var page = getSecondPage();
                    console.debug("pushing detail " + page)
                    navigationPane.push(page);
                }
                property Page secondPage
                function getSecondPage() {
                    if (! secondPage) {
                        secondPage = secondPageDefinition.createObject();
                    }
                    return secondPage;
                }
                attachedObjects: [
                    ComponentDefinition {
                        id: secondPageDefinition
                        source: "DetailsPage.qml"
                    }
                ]
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
