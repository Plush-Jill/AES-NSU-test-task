import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    minimumWidth: 500
    minimumHeight: 400
    title: "SCPI Client"

    ColumnLayout {
        anchors.margins: 20
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            ComboBox {
                id: commandComboBox
                model: backend.get_commands()
                implicitWidth: 200
            }

            Button {
                text: "Send"
                enabled: commandComboBox.currentText !== ""
                onClicked: {
                    try {
                        backend.send_command(commandComboBox.currentText)
                    } catch (error) {
                        console.error("Error executing command:", error)
                    }
                }
            }

            Button {
                text: "Reconnect"
                onClicked: {
                    try {
                        backend.reconnect()
                    } catch (error) {
                        console.error("Reconnect error:", error)
                    }
                }
            }

            Item { Layout.fillWidth: true }
        }
    }

    TextArea {
        id: resultOutput
        anchors {
            left: parent.left
            bottom: commandInput.top
            margins: 20
        }
        width: parent.width / 2
        height: parent.height / 2
        readOnly: true
        text: backend.get_history()
        placeholderText: "History:\n"
    }

    TextField {
        id: commandInput
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 20
        }
        width: 300
        height: 30
        placeholderText: "Enter command here, enter to send"
        onAccepted: {
            if (text.length > 0) {
                backend.send_command(text)
                text = ""
            }
        }
    }

    Connections {
        target: backend
        function onHistoryChanged(history) {
            resultOutput.text = history
        }
    }
}