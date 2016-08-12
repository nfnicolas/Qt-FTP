#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>
#include <QStringList>
#include <QTimer>
#include <QUrl>

class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = 0);
    void doDownload(const QUrl &url);
    QString saveFileName(const QUrl &url);
    bool saveToDisk(const QString &fileName, QIODevice *data);

signals:

public slots:
    void execute();
    void downloadFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);
private:
    QNetworkAccessManager manager;
    QList<QNetworkReply*> currentFtpDownloads;
};

#endif // CLIENT_H
