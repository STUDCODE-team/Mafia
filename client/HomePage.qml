import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page{

    ColumnLayout{
        anchors.centerIn: parent

        Button{
            implicitWidth: 200
            text:  qsTr("Создать комнату")
            onClicked: {
                manager.createNewRoom()
            }
        }

        Button{
            implicitWidth: 200
            text:  qsTr("Подключиться")
            onClicked: {
                main.setPage(connectToRoomPage)
            }
        }
    }
}
