import QtQuick 2.12
import QtQuick.Controls 2.5
import com.duoduo.components 1.0

Page {
    width: 600
    height: 400

    header: Label {
        text: qsTr("Page 2")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

QtQuick2QREncode {
    id:qr
//        anchors.fill: parent
    width: 300
    height: 300
    qrSize: Qt.size(width,width)
    anchors.centerIn: parent
    qrData:"www.bluedrum.cn" //encode contents
    qrForeground:  "black" // "#29aee1" //encode color
    qrBackground: "#dddddd"
    qrMargin: 10
    qrMode: QtQuick2QREncode.MODE_8    //encode model
    qrLevel: QtQuick2QREncode.LEVEL_Q // encode level
    //qrLogo: "qrc:/256.png" //or local path or qrc path but not network url
    onQrSaveFileChanged: {
        console.log("We get save file path is :"+qrfilepath)
    }
    onHeightChanged: {
        getSize();
    }
    onWidthChanged: {
       getSize();
    }
}
}
