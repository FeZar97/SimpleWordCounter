import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

RowLayout {
    id: rowLayout
    Layout.fillWidth: true
    Layout.margins: 5
    spacing: 5

    function updateStatistics(totalWordsNb, processingTimeSec) {

        totalWordsLabel.text = qsTr("Total words: ") + String(totalWordsNb).replace(/(\d)(?=(\d{3})+([^\d]|$))/g, "$1 ")

        // var h = processingTimeSec/3600^0
        // var m = (processingTimeSec-h*3600)/60^0
        // var s = processingTimeSec-h*3600-m*60
        // var timeString = (h<10?"0"+h:h)+" h. "+(m<10?"0"+m:m)+" min. "+(s<10?"0"+s:s)+" sec."
        var timeString = processingTimeSec + "s."
        processingTimeLabel.text = qsTr("Processing time: ") + timeString
    }

    Label {
        id: totalWordsLabel
        Layout.minimumWidth: rowLayout.parent.width / 2
        text: qsTr("Total words: ")
        font.family: "Courier new"
        font.pointSize: 13
    }

    Label {
        id: processingTimeLabel
        text: qsTr("Processing time: ")
        font.family: "Courier new"
        font.pointSize: 13
    }

    Component.onCompleted: {
        updateStatistics(0, 0)
    }
}
