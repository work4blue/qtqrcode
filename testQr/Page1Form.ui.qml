import QtQuick 2.12
import QtQuick.Controls 2.5


Page {
    width: 600
    height: 400

    header: Label {
        text: qsTr("Page 1")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Text {
        id:mResult
        text: qsTr("打描结果")
        height:300
        width:300

        anchors.centerIn: parent
    }

    Connections{
        target: AndroidActivity
        ignoreUnknownSignals: true
        onQrcodeDataChanged:{
            mResult.text = AndroidActivity.qrcodeData;
        }
    }

    Button {
        text:qsTr("开始扫描")
        onClicked:{
        AndroidActivity.callQrScan();
        }
//        onClicked:{
//            AndroidActivity.callQrScan();
//        }

       anchors.bottom: parent.bottom
       anchors.bottomMargin: 20
       anchors.horizontalCenter: parent.horizontalCenter
    }
}
