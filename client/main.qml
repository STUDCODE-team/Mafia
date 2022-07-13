import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("MAFIA")
    Connections{
        target: manager
        function onSetConnectionStatus(status)
        {
            connectionLabel.text = status
        }
        function onRoomCreated(key)
        {
            roomNumber.key = key
        }
    }

    ColumnLayout {
        anchors.fill: parent
        Label{
            id: connectionLabel
            Layout.fillWidth: true
            text: "Connecting..."
        }
        Label {
            id: roomNumber
            property string key
            text: "Комната №" + key;
            Layout.fillWidth: true
            color: "blue"
        }
        Button{
            Layout.fillWidth: true
            text: "Создать комнату"
            onClicked: {
                manager.createNewRoom();
            }
        }

    }
}
