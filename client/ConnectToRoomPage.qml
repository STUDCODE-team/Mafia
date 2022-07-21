import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: page

    function setInputFieldFocus(){
        inpt.forceActiveFocus();
        console.log("focus")
    }

    RowLayout{
        Button{
            implicitHeight: 30
            implicitWidth: 80
            text: qsTr("<- Выход")
            onClicked: {
                inpt.text = "";
                main.setPage(homePage)
            }
        }
        Item{Layout.fillWidth: true}
    }

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
                maximumLength: 4;
            }
        }

        Button{
            implicitWidth: 200
            text:  qsTr("Подключиться")
            onClicked: {
                manager.connectToRoom(inpt.text);
            }
        }
    }
}
