import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {

    id: main
    width: 640
    height: 480
    visible: true
    title: qsTr("MAFIA")

    Connections{
        target: manager
        function onQml_enterRoom(num){
            setPage(roomLobby)
            roomLobby.setRoomNum(num)
        }
    }

    function setPage(page){
        stack.currentIndex = page.stackIndex
    }

    StackLayout{
        id: stack
        anchors.fill: parent
        HomePage{
            id: homePage
            property int stackIndex: 0
        }
        ConnectToRoomPage{
            id: connectToRoomPage
            property int stackIndex: 1
        }
        RoomLobby{
            id: roomLobby
            property int stackIndex: 2
        }
    }
}
