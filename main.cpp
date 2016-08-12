#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    client ftpClient;
    QTimer::singleShot(0,&ftpClient,SLOT(execute()));

    return a.exec();
}

