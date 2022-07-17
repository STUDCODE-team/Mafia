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

    function setPage(page){
        pageLoader.setSource(page)
    }

    Component.onCompleted: {
        pageLoader.setSource("HomePage.qml")
    }

    Loader{
        id: pageLoader
        anchors.fill: parent
    }
}
