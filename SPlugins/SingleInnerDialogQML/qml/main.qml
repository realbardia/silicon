import QtQuick 1.0
Rectangle{
    width: 500
    height: 500
    color: "#00000000"
    objectName: "background"

    ShadowRectangle {
        objectName: "dialog"
        id: back
        state: "Closed"
        smooth: true
        height: 0
        color: "transparent"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        property int temp_height

        grad: Gradient {
            GradientStop { id: gradient1; position: 0.0; color: "#666666" }
            GradientStop { id: gradient2; position: 0.1; color: "#ffffff" }
        }

        Behavior on height {
            NumberAnimation
            {
                duration: 337
                easing.type: Easing.OutBack
            }
        }

        states: [
            State {
                name: "Opened"
                PropertyChanges { target: back; height: back.temp_height }
            },
            State {
                name: "Closed"
                PropertyChanges { target: back; height: 0 }
            }
        ]
    }

}
