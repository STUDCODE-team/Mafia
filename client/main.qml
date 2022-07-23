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
        function onQml_exitRoom(){
            setPage(homePage)
        }

        function onQml_enterRoom(num, playerList){
            setPage(roomLobby)
            roomLobby.updateData(num, playerList)
        }
    }

    function setPage(page){
        stack.currentIndex = page.stackIndex
    }

    StackLayout{
        id: stack
        anchors.fill: parent
        onCurrentIndexChanged: {
            if (currentIndex == connectToRoomPage.stackIndex){
                connectToRoomPage.setInputFieldFocus()
            }else{
                forceActiveFocus();
            }

            if(currentIndex == connectToRoomPage.stackIndex){
                manager.catchNFC();
            }else if(currentIndex == roomLobby.stackIndex){
                manager.broatcastNFC();
            }else {
                manager.offNFC();
            }
        }
        Page{
            id: loadingPage
            property int stackIndex: 0
            //типо экран загрузки
        }
        HomePage{
            id: homePage
            property int stackIndex: 1
        }
        ConnectToRoomPage{
            id: connectToRoomPage
            property int stackIndex: 2
        }
        RoomLobby{
            id: roomLobby
            property int stackIndex: 3
        }
    }
}
