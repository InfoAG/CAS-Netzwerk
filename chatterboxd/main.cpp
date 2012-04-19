#include "ChatterBoxServer.h"
#include <QCoreApplication>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    ChatterBoxServer *server = new ChatterBoxServer();
    int port;
    QString input;
    cout << "Port [4200]:\t";
    input = QTextStream(stdin).readLine();

    bool success;
    if (input.isEmpty()) success = server->listen(QHostAddress::Any, 4200);
    else success = server->listen(QHostAddress::Any, input.toInt());

    if(!success)
    {
        qFatal("Could not listen on port 4200.");
    }

    qDebug() << "Ready";

    return app.exec();
}
