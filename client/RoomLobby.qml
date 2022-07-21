import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml.Models 2.15

Item {

    function updateData(roomNum, players){
        roomLabel.text = roomNum
        playerModel.clear()
        for(var id in players){
            playerModel.append({"playerID": players[id]})
        }
    }

    ListModel{
        id: playerModel
    }

    ColumnLayout{
        anchors.fill: parent
        RowLayout{
            Layout.fillWidth: true
            Button{
                implicitHeight: 30
                implicitWidth: 80
                text: qsTr("<- Выход")
                onClicked: {
                    manager.exitRoom();
                }
            }
            Label{
                Layout.fillWidth: true
                id: roomLabel
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
        ListView{
            id: playerListView
            Layout.fillHeight: true
            model: playerModel
            delegate: ItemDelegate{
                text: modelData
            }
        }
    }
}
