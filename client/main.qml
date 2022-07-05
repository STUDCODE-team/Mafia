import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Chat Client")

    Connections {
        target: client
        function onNewMessage(ba) {
            listModelMessages.append({
                                         message: ba + ""
                                     })
        }
        function onSetConnectionStatus(status) {
            statusLabel.text = status
        }
    }
    ColumnLayout {
        anchors.fill: parent
        Label {
            id: statusLabel
            Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: "Connecting to server..."
        }
        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            model: ListModel {
                id: listModelMessages
                ListElement {
                    message: "Welcome to chat client"
                }
            }
            delegate: ItemDelegate {
                text: message
            }
            ScrollBar.vertical: ScrollBar {}
        }
        RowLayout {
            Layout.fillWidth: true
            Button {
                id: buttonSend
                text: qsTr("Send")
                onClicked: {
                    client.send(textFieldMessage.text)
                    textFieldMessage.clear()
                }
            }
        }
    }
}
