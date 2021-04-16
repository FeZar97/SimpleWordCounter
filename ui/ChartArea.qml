import QtQuick 2.12
import QtCharts 2.3
import QtQuick.Layouts 1.12

RowLayout {
    id: rowLayout
    Layout.margins: 5
    spacing: 5
    Layout.fillWidth: true
    Layout.fillHeight: true

    ChartView {
        id: chartView
        Layout.fillHeight: true
        Layout.fillWidth: true
        title: "Most popular words"
        legend.visible: false

        titleFont.family: "Courier new"
        titleFont.pointSize: 15
        titleFont.bold: true

        theme: ChartView.ChartThemeDark
        antialiasing: true

        ValueAxis {
            id: valueAxisY
            min: 0
            max: 10
            tickCount: 10
            labelFormat: "%d"
            titleText: "Words number"
        }

        BarCategoryAxis {
            id: barCategoriesAxis
        }

        BarSeries {
            id: barSeries
            axisX: barCategoriesAxis
            axisY: valueAxisY
            labelsVisible: true
            labelsPosition: AbstractBarSeries.LabelsOutsideEnd

            BarSet {
                id: wordsSet
                label: "Words"
                labelFont: {
                    color: "black";
                }
            }
        }
    }

    function updateWordsStatistic(variantList) {

        wordsSet.values = [];

        chartView.title = variantList.length + " most popular words";
        var xLabels = barCategoriesAxis.categories;

        var max = 0;
        for (var i = 0; i < variantList.length; i++) {
            if(variantList[i][1] > max) {
                max = variantList[i][1];
            }
        }

        valueAxisY.max = (Math.floor(max * 1.1 / 10) + 1) * 10;

        for (var wordIdx = 0; wordIdx < variantList.length; wordIdx++) {
            wordsSet.append(variantList[wordIdx][1]);
            xLabels[Number(wordIdx)] = variantList[wordIdx][0];
        }

        barCategoriesAxis.categories = xLabels;
        barCategoriesAxis.visible = true;
        barCategoriesAxis.min = 0;
        barCategoriesAxis.max = xLabels.length - 1;
    }
}
