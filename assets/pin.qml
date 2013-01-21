import bb.cascades 1.0

Container {
    // File path of the pin image
    property string pinImageSource: "asset:///images/pin.png"
    // pointerOffsetX, pointerOffsetY is the position of the pixel in pin image that should point to the location. Change these to match your pin image.
    property int pointerOffsetX: 20
    property int pointerOffsetY: 58
    /////////////////////////////////////////////////////////
    id: root
    property int x: 0
    property int y: 0
    property double lat
    property double lon
    property alias animDrop: animDrop
    clipContentToBounds: false
    overlapTouchPolicy: OverlapTouchPolicy.Allow
    
    layoutProperties: AbsoluteLayoutProperties {
        id: position
        positionX: x - pointerOffsetX
        positionY: y - pointerOffsetY
    }
    ImageView {
        id: pinImage
        scaleX: .8
        scaleY: .8
        imageSource: pinImageSource
        focusPolicy: FocusPolicy.Touch
        overlapTouchPolicy: OverlapTouchPolicy.Allow
        onFocusedChanged: {
            if (focused) {
                animFocus.play();                               
            }
            if (! focused) {                
                animUnfocus.play();                
            }
        }
        animations: [
            ScaleTransition {
                id: animFocus
                fromX: .8
                toX: 1
                fromY: .8
                toY: 1
                duration: 300
                easingCurve: StockCurve.BounceOut
            },
            ScaleTransition {
                id: animUnfocus
                fromX: 1
                toX: .8
                fromY: 1
                toY: .8
                duration: 300
                easingCurve: StockCurve.BounceOut
            }
        ]
    }
    animations: [
        TranslateTransition {
            id: animDrop
            fromY: - position.positionY
            toY: 0
            duration: 600
            easingCurve: StockCurve.BounceOut
        }
    ]
}
