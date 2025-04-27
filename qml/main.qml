import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "SCPI Client"

    Column {
        spacing: 10
        anchors.margins: 20
        anchors.fill: parent

        ComboBox {
            id: commandList
            width: parent.width
            model: []
            textRole: "command"
            onActivated: {
                console.log("Selected command:", currentText)
                // Здесь можно вызвать метод для отправки команды
                backend.sendCommand(currentText)
            }
        }

        TextArea {
            id: resultOutput
            width: parent.width
            height: parent.height - commandList.height - 20
            readOnly: true
            placeholderText: "Результат выполнения команды будет отображён здесь"
        }
    }
}