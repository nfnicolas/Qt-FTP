#include "client.h"

client::client(QObject *parent) : QObject(parent)
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)),SLOT(downloadFinished(QNetworkReply*)));
}

void client::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply=manager.get(request);

#ifndef QT_NO_SSL
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),SLOT(sslErrors(QList<QSslError>)));
#endif

    currentFtpDownloads.append(reply);
}

QString client::saveFileName(const QUrl &url)
{
    QString path=url.path();
    QString basename=QFileInfo(path).fileName();

    if(basename.isEmpty())
    {
        basename="reply";
    }

    if(QFile::exists(basename))
    {
        int i=0;
        basename+='.';
        while(QFile::exists(basename+QString::number(i)))
        {
            i++;
        }

        basename+=QString::number(i);
    }

    return basename;
}

bool client::saveToDisk(const QString &fileName, QIODevice *data)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

void client::execute()
{
    QStringList args=QCoreApplication::instance()->arguments();
    args.takeFirst();

    if(args.isEmpty())
    {
        QCoreApplication::instance()->quit();
        return;
    }

    foreach(QString arg, args)
    {
        QUrl url=QUrl::fromEncoded(arg.toLocal8Bit());
        doDownload(url);
    }
}

void client::downloadFinished(QNetworkReply *reply)
{
    QUrl url=reply->url();

    if(reply->error())
    {
        fprintf(stdout,"Download of %s failed: %s\n",qPrintable(url.toString()),qPrintable(reply->errorString()));
    }
    else
    {
        QString fileName=saveFileName(url);
        if(saveToDisk(fileName,reply))
        {
            fprintf(stdout,"Download of %s succeeded (saved to %s)\n",
                    qPrintable(url.toString()),
                    qPrintable(fileName));
        }
    }

    currentFtpDownloads.removeAll(reply);
    reply->deleteLater();

    if(currentFtpDownloads.isEmpty())
    {
        QCoreApplication::instance()->quit();
    }
}

void client::sslErrors(const QList<QSslError> &errors)
{
#ifndef QT_NO_SSL
    foreach (const QSslError &error, errors)
    {
        fprintf(stdout,"SSL error: %s\n",qPrintable(error.errorString()));
    }
#else
    Q_UNUSED(errors);
#endif
}







