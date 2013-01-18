// Navigation pane project template
import bb.cascades 1.0

Page {
    // page with a picture detail
    id: pgDetail
    actions: [
        // create navigation panel actions here
        ActionItem {
            title: qsTr("Break")
            onTriggered: {
                imgView.imageSource = "asset:///images/picture1br.png"
            }
        }
    ]
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            onTriggered: {
                // define what happens when back button is pressed here
                // in this case is closed the detail page
                navigationPane.pop()
            }
        }
    }
    Container {
        background: Color.Black
        Label {
            text: qsTr("Page 2")
            horizontalAlignment: HorizontalAlignment.Center
            textStyle {
                base: SystemDefaults.TextStyles.TitleText
                color: Color.Yellow
            }
        }
        ImageView {
            id: imgView
            imageSource: "asset:///images/picture1.png"
            horizontalAlignment: HorizontalAlignment.Center
        }
        Label {
            text: qsTr("Picture description")
            horizontalAlignment: HorizontalAlignment.Center
        }
    }
}
