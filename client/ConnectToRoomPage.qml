import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {

    RowLayout{
        anchors.centerIn: parent

        Rectangle{
            implicitWidth: 200
            implicitHeight: 40
            border.color: "grey"
            border.width: 2

            TextInput{
                id: inpt
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Button{
            implicitWidth: 200
            text:  qsTr("Подключиться")
            onClicked: {
                //connect to room(inpt)
            }
        }
    }
}
