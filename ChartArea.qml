import QtQuick 2.12
import QtCharts 2.3
import QtQuick.Layouts 1.12

RowLayout {
    id: rowLayout
    Layout.margins: 5
    spacing: 5
    Layout.fillHeight: true

    function updateWordsStatistic(variantList) {
        pieSeries.clear()

        chartView.title = variantList.length + " most popular words"

        for (var i = 0; i < variantList.length; i++) {

            if(variantList[i].length === 2) {
                pieSeries.append(variantList[i][0] + " (" + variantList[i][1] + ")",
                                 variantList[i][1])
            }
        }
        for (var j = 0; j < pieSeries.count; j++) {
            pieSeries.at(j).labelVisible = true
        }
    }

    ChartView {
        id: chartView
        Layout.fillHeight: true
        Layout.fillWidth: true
        title: "Most popular words"
        legend.visible: false
        titleFont.family: "Courier new"
        titleFont.pointSize: 15
        titleFont.bold: true
        theme: ChartView.ChartThemeQt

        antialiasing: true

        PieSeries {
            id: pieSeries
        }
    }
}
