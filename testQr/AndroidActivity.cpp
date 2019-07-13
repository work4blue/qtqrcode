#include "AndroidActivity.h"

#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>
#endif

//参考
// 1. https://github.com/minixxie/examples/tree/master/qt-for-mobile/TestExternalAndroidCamera1
// 2. https://blog.csdn.net/foruok/article/details/43560437
// 3. http://bcoder.com/java/use-tools-replace-in-android

#include <QDebug>

#define  SHOOT_PHOTO 100
#define  SCAN_QRCODE 101

#define CHECK_EXCEPTION(env) \
if(env->ExceptionCheck())\
{\
qDebug() << "exception occured";\
env->ExceptionClear();\
}

//https://blog.csdn.net/foruok/article/details/43560437

AndroidActivity::AndroidActivity(QObject *parent) : QObject(parent)
{

}

void AndroidActivity::callExternalCamera()
{
#ifdef Q_OS_ANDROID
    //android.provider.MediaStore.EXTRA_OUTPUT
    QAndroidJniObject MediaStore__EXTRA_OUTPUT
            = QAndroidJniObject::getStaticObjectField(
                "android/provider/MediaStore", "EXTRA_OUTPUT", "Ljava/lang/String;");
    qDebug() << "MediaStore__EXTRA_OUTPUT.isValid()=" << MediaStore__EXTRA_OUTPUT.isValid();

    QAndroidJniObject action = QAndroidJniObject::fromString("android.media.action.IMAGE_CAPTURE");
    QAndroidJniObject intent=QAndroidJniObject("android/content/Intent","(Ljava/lang/String;)V",
                                               action.object<jstring>());
    qDebug() << __FUNCTION__ << "intent.isValid()=" << intent.isValid();

    QAndroidJniObject extDir = QAndroidJniObject::callStaticObjectMethod(
                "android/os/Environment", "getExternalStorageDirectory", "()Ljava/io/File;");
    qDebug() << "extDir.isValid()=" << extDir.isValid();

    QAndroidJniObject filename = QAndroidJniObject::fromString("camera.jpg");

    QAndroidJniObject photo=QAndroidJniObject("java/io/File","(Ljava/io/File;Ljava/lang/String;)V",
                                               extDir.object<jobject>(), filename.object<jstring>());
    qDebug() << __FUNCTION__ << "photo.isValid()=" << photo.isValid();

    takePhotoSavedUri = QAndroidJniObject::callStaticObjectMethod(
                "android/net/Uri", "fromFile", "(Ljava/io/File;)Landroid/net/Uri;", photo.object<jobject>());
    qDebug() << "takePhotoSavedUri.isValid()=" << takePhotoSavedUri.isValid();

    intent.callObjectMethod(
                "putExtra","(Ljava/lang/String;Landroid/os/Parcelable;)Landroid/content/Intent;",
                MediaStore__EXTRA_OUTPUT.object<jstring>(), takePhotoSavedUri.object<jobject>());
    qDebug() << __FUNCTION__ << "intent.isValid()=" << intent.isValid();


    QtAndroid::startActivity(intent, SHOOT_PHOTO, this);
#endif
}
/*
 * 打开二维码扫描窗口
 */
void AndroidActivity::callQrScan()
{
 #ifdef Q_OS_ANDROID

    requestCameraPerm();
    requestSdcardAccess();

  /*
   * 相当于java 如下代码
   *  Intent intent = new Intent("bluedrum.scan.qrcode");

        context.startActivityForResult(intent,activity_id);
   */

    QAndroidJniObject action = QAndroidJniObject::fromString("bluedrum.scan.qrcode");
    QAndroidJniObject intent("android/content/Intent","(Ljava/lang/String;)V", action.object<jstring>());

    qDebug() << "call qr scan";
    QtAndroid::startActivity(intent, SCAN_QRCODE, this);

    //或者单独建一个recv也行
    //ResultReceiver *resultReceiver =
    //        new ResultReceiver(savedImageFile.toString(), m_capturedImage);
    //startActivity(intent, 1, resultReceiver)

#endif
}

bool AndroidActivity::requestCameraPerm()
{

    return requestPermisson("android.permission.CAMERA");

}

bool AndroidActivity::requestSdcardAccess()
{
     requestPermisson("android.permission.READ_EXTERNAL_STORAGE");
     return requestPermisson("android.permission.WRITE_EXTERNAL_STORAGE");

}

bool AndroidActivity::requestPermisson(const QString &perm)
{
    QtAndroid::PermissionResult r = QtAndroid::checkPermission(perm);
    if(r == QtAndroid::PermissionResult::Denied) {
        QtAndroid::requestPermissionsSync( QStringList() << perm );
        r = QtAndroid::checkPermission(perm);
        if(r == QtAndroid::PermissionResult::Denied) {
             return false;
        }
   }
    return true;
}



void AndroidActivity::requestPermissons()
{



}
#ifdef Q_OS_ANDROID

#define  W4B_Activity_RESULT_CANCELED     (0)
#define W4B_Activity_RESULT_OK   (-1)


void AndroidActivity::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data)
{
    qDebug() << "receiverRequestCode "<<receiverRequestCode<<",resultCode="<<resultCode;

//        jint Activity__RESULT_OK = QAndroidJniObject::getStaticField<jint>(
//                    "android.app.Activity", "RESULT_OK");

//        if ( receiverRequestCode == SHOOT_PHOTO && resultCode == W4B_Activity_RESULT_OK )
//        {
//            qDebug() << "takePhotoSavedUri:" << takePhotoSavedUri.toString();

//            QAndroidJniObject absPath = takePhotoSavedUri.callObjectMethod("getPath","()Ljava/lang/String;");
//            qDebug() << __FUNCTION__ << "absPath.isValid()=" << absPath.isValid();

//            m_imagePath = absPath.toString();
//            emit this->imagePathChanged();
//        }
//        else
    if ( receiverRequestCode == SCAN_QRCODE && resultCode == W4B_Activity_RESULT_OK ){

            if(!data.isValid()){
                qDebug() << "scan result no isValid";
                return ;
            }
               QAndroidJniObject param1 = QAndroidJniObject::fromString("result");
               QAndroidJniObject jniResult = data.callObjectMethod("getStringExtra","(Ljava/lang/String;)Ljava/lang/String;",param1.object<jstring>());

            m_qrcodeData =  jniResult.toString();

////            //相当于java intent.getStringExtra("result");
            qDebug() << "scan result  "<<m_qrcodeData;
            emit qrcodeDataChanged();
       }
}
#endif

//https://stackoverflow.com/questions/34351644/startactivity-on-qt-nothing-displays
/*
QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");   //activity is valid
if ( activity.isValid() )
{
    // Equivalent to Jave code: 'Intent intent = new Intent();'
    QAndroidJniObject intent("android/content/Intent","()V");
    if ( intent.isValid() )
    {
        QAndroidJniObject param1 = QAndroidJniObject::fromString("com.android.settings");
        QAndroidJniObject param2 = QAndroidJniObject::fromString("com.android.settings.DevelopmentSettings");

        if ( param1.isValid() && param2.isValid() )
        {
            // Equivalent to Jave code: 'intent.setClassName("com.android.settings", "com.android.settings.DevelopmentSettings");'
            intent.callObjectMethod("setClassName","(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",param1.object<jobject>(),param2.object<jobject>());

            // Equivalent to Jave code: 'startActivity(intent);'
            activity.callObjectMethod("startActivity","(Landroid/content/Intent;)V",intent.object<jobject>());
        }
    }
}
 */
//比较复杂取Intent  https://stackoverflow.com/questions/34163437/qt-and-android-gallery-mediastore-using-qandroidjniobject
/*
 * void imagePickerAndroid::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data)
{
    jint RESULT_OK = QAndroidJniObject::getStaticField<jint>("android/app/Activity", "RESULT_OK");
    if (receiverRequestCode == 101 && resultCode == RESULT_OK)
    {
        QAndroidJniObject uri = data.callObjectMethod("getData", "()Landroid/net/Uri;");
        QAndroidJniObject dadosAndroid = QAndroidJniObject::getStaticObjectField("android/provider/MediaStore$MediaColumns", "DATA", "Ljava/lang/String;");
        QAndroidJniEnvironment env;
        jobjectArray projecao = (jobjectArray)env->NewObjectArray(1, env->FindClass("java/lang/String"), NULL);
        jobject projacaoDadosAndroid = env->NewStringUTF(dadosAndroid.toString().toStdString().c_str());
        env->SetObjectArrayElement(projecao, 0, projacaoDadosAndroid);
        QAndroidJniObject contentResolver = QtAndroid::androidActivity().callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");
        QAndroidJniObject cursor = contentResolver.callObjectMethod("query", "(Landroid/net/Uri;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Landroid/database/Cursor;", uri.object<jobject>(), projecao, NULL, NULL, NULL);
        jint columnIndex = cursor.callMethod<jint>("getColumnIndex", "(Ljava/lang/String;)I", dadosAndroid.object<jstring>());
        cursor.callMethod<jboolean>("moveToFirst", "()Z");
        QAndroidJniObject resultado = cursor.callObjectMethod("getString", "(I)Ljava/lang/String;", columnIndex);
        QString imagemCaminho = "file://" + resultado.toString();
        emit imagemCaminhoSignal(imagemCaminho);
    }
    else
    {
        qDebug() << "Caminho errado";
    }
}
 */
/*
QAndroidJniObject Package;
QAndroidJniObject Subject;
QAndroidJniObject Text;
QAndroidJniObject intent("android/content/Intent");

Scanner = QAndroidJniObject::fromString("com.google.zxing.client.android.SCAN");
Package = QAndroidJniObject::fromString("com.google.zxing.client.android");
Subject = QAndroidJniObject::fromString("SCAN_MODE");
Text = QAndroidJniObject::fromString("QR_CODE_MODE");
intent.callObjectMethod("addAction","(Ljava/lang/String;)Landroid/content/Intent;",Scanner.object<jstring>());
intent.callObjectMethod("setPackage","(Ljava/lang/String;)Landroid/content/Intent;",Package.object<jstring>());
intent.callObjectMethod("putExtra","(Ljava/lang/String;Ljava/lang/string;)Landroid/content/Intent;",Subject.object<jstring>(), Text.object<jstring>());
QtAndroid::startActivity(intent,123456);@
 */
