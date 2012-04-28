#include "ChatterBoxServer.h"
#include <QCoreApplication>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    ChatterBoxServer *server = new ChatterBoxServer();
    int port;
    QString input;
    QTextStream qtin(stdin);
    bool success;

    for (;;) {
        cout << "Port [4200]:\t";
        input = qtin.readLine();

        if (input.isEmpty()) port = 4200;
        else port = input.toInt();

        if (server->listen(QHostAddress::Any, port)) break;
        else qWarning() << "Could not listen on port " << port << "\b. Please try a different one.";
    }

    qDebug() << "Ready";

    return app.exec();
}
