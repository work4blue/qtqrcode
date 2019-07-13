#ifndef ANDROIDACTIVITY_H
#define ANDROIDACTIVITY_H

#include <QObject>
#ifdef Q_OS_ANDROID
    #include <QAndroidActivityResultReceiver>
#endif

#ifdef Q_OS_ANDROID
class AndroidActivity : public QObject, QAndroidActivityResultReceiver
#else
class AndroidActivity : public QObject
#endif
{
    Q_OBJECT
public:
    explicit AndroidActivity(QObject *parent = nullptr);
    Q_PROPERTY(QString imagePath READ imagePath NOTIFY imagePathChanged)
    Q_PROPERTY(QString qrcodeData READ qrcodeData NOTIFY qrcodeDataChanged)
private:
    QString m_imagePath;
    QString m_qrcodeData;
public://getters/setters
    const QString& imagePath() const { return this->m_imagePath; }
    const QString& qrcodeData() const { return this->m_qrcodeData; }

public:
    Q_INVOKABLE void callExternalCamera();
    Q_INVOKABLE void callQrScan();

    Q_INVOKABLE void requestPermissons();

    bool requestCameraPerm();
    bool requestSdcardAccess();
    bool requestPermisson(const QString&perm);

signals:
    void imagePathChanged();
    void qrcodeDataChanged();

private:
#ifdef Q_OS_ANDROID
    QAndroidJniObject takePhotoSavedUri;
    void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject & data);
#endif
signals:

public slots:
};

#endif // ANDROIDACTIVITY_H
