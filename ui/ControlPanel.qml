import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

RowLayout {
    id: rowLayout
    Layout.margins: 5
    Layout.fillWidth: true
    spacing: 5

    function setStartEnabled(state) {
        importButton.enabled = state
        startProcessButton.enabled = state
        if(selectedFile.text == "") {
            startProcessButton.enabled = false
        }

        stopProcessButton.enabled = !state
    }

    FileDialog {
        id: fileOpenDialog
        title: qsTr("Open file")
        nameFilters: [ "All files (*)" ]
        folder: shortcuts.desktop
        onAccepted: {
            var selectedFileStr = fileOpenDialog.fileUrl.toString()
            selectedFile.text = selectedFileStr.replace('file:///','');
            setStartEnabled(selectedFileStr.length > 0)
        }
    }

    TextField {
        id: selectedFile
        Layout.fillWidth: true
        font.family: "Courier new"
        font.pointSize: 13
        readOnly: true

        placeholderText: qsTr("Selected file")
        background: Rectangle {
            implicitWidth: selectedFile.width
            implicitHeight: selectedFile.height
            color: selectedFile.enabled ? "transparent" : "#353637"
            border.color: selectedFile.text.length > 0 ? "#21be2b" : "#e0e0e0"
        }
    }

    Button {
        id: importButton
        implicitHeight: selectedFile.height
        font.family: "Courier new"
        font.pointSize: 13
        text: qsTr("Choose file")
        onClicked: fileOpenDialog.open()
    }

    Button {
        id: startProcessButton
        implicitHeight: selectedFile.height
        font.family: "Courier new"
        font.pointSize: 13
        text: qsTr("Start")
        onClicked: {
            wordCounterProxy.startProcessFileUI(selectedFile.text)
            setStartEnabled(false)
        }
    }

    Button {
        id: stopProcessButton
        implicitHeight: selectedFile.height
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
