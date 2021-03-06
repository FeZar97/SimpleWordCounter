import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

RowLayout {
    id: rowLayout
    Layout.margins: 5
    Layout.fillWidth: true
    spacing: 5

    function setProgress(curProgress) {
        progressBar.value = curProgress
        progressValueLabel.text = (curProgress * 100).toFixed(1) + "%"
    }

    Label {
        id: progressLabel
        text: qsTr("Progress")
        font.family: "Courier new"
        font.pointSize: 13
    }

    ProgressBar {
        id: progressBar
        value: 0.0
        padding: 2
        Layout.fillWidth: true

        background: Rectangle {
            implicitHeight: 6
            implicitWidth: progressBar.width
            color: "#e6e6e6"
            radius: 3
        }

        contentItem: Item {
            implicitHeight: 12

            Rectangle {
                width: progressBar.visualPosition * progressBar.width
                height: parent.height
                radius: 2
                color: "#17a81a"
            }
        }
    }

    Label {
        id: progressValueLabel
        font.family: "Courier new"
        font.pointSize: 13
    }

    Component.onCompleted: {
        setProgress(0)
    }
}
