import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {

    function setRoomNum(num){
        roomLabel.text = num
    }

    ColumnLayout{
        anchors.fill: parent
        Label{
            id: roomLabel
            Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        ListView{
            Layout.fillHeight: true
        }
    }
}
