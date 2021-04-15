import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.10
import QtQuick.Controls 2.15

Window {
    visible: true
    width: Screen.width / 1.8
    height: Screen.height / 1.8
    title: qsTr("SimpleWordCounter")

    Connections {
        target: wordCounterProxy
        function onSetProgressUI(curProgress) {
            progressBarPanel.setProgress(curProgress)
        }
        function onUpdateDataUI(data) {
            chartArea.updateWordsStatistic(data)
        }
        function onUpdateStatistics(totalWordsNb, processingTimeSec) {
            statisticPanel.updateStatistics(totalWordsNb, processingTimeSec)
        }

        function onFinished() {
            controlPanel.setStartEnabled(true)
        }
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent

        ControlPanel {
            id: controlPanel
        }

        ChartArea {
            id: chartArea
        }

        StatisticPanel {
            id: statisticPanel
        }

        ProgressBarPanel {
            id: progressBarPanel
        }
    }

    Component.onCompleted: {
        progressBarPanel.setProgress(0)
        controlPanel.setStartEnabled(true)
        statisticPanel.updateStatistics(0, 0)
    }
}
