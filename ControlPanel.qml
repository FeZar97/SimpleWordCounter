import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

RowLayout {
    id: rowLayout
    Layout.margins: 5
    Layout.fillWidth: true
    spacing: 5

    property string selectedFileName: ""

    function setStartEnabled(state) {
        importButton.enabled = state
        startProcessButton.enabled = state
        stopProcessButton.enabled = !state
    }

    FileDialog {
        id: fileOpenDialog
        title: qsTr("Open file")
        nameFilters: [ "All files (*)" ]
        folder: shortcuts.desktop
        onAccepted: {
            selectedFileName = fileOpenDialog.fileUrl
        }
    }

    Button {
        id: importButton
        font.family: "Courier new"
        font.pointSize: 13
        text: qsTr("Choose file")
        onClicked: fileOpenDialog.open()
    }

    Button {
        id: startProcessButton
        font.family: "Courier new"
        font.pointSize: 13
        text: qsTr("Start")
        onClicked: {
            wordCounterProxy.startProcessFileUI(selectedFileName)
            setStartEnabled(false)
        }
    }

    Button {
        id: stopProcessButton
        font.family: "Courier new"
        font.pointSize: 13
        text: qsTr("Stop")
        onClicked: {
            wordCounterProxy.stopProcessFileUI()
            setStartEnabled(true)
        }
    }

    Component.onCompleted: {
        setStartEnabled(true)
    }
}
