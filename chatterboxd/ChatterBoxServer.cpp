#include "ChatterBoxServer.h"

#include <QTcpSocket>
#include <QRegExp>

ChatterBoxServer::ChatterBoxServer(QObject *parent) : QTcpServer(parent)
{
}

void ChatterBoxServer::incomingConnection(int socketfd)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketfd);
    clients.insert(client);

    qDebug() << "New client from:" << client->peerAddress().toString();

    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void ChatterBoxServer::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    while(client->canReadLine())
    {
        QString line = QString::fromUtf8(client->readLine()).trimmed();
        qDebug() << "Read line:" << line;

        QRegExp meRegex("^/me:(.*)$");

        if(meRegex.indexIn(line) != -1)
        {
            QString user = meRegex.cap(1);
            users[client] = user;
            foreach(QTcpSocket *client, clients)
                client->write(QString("Server:" + user + " has joined.\n").toUtf8());
            sendUserList();
        }
        else if(users.contains(client))
        {
            QString message;
            QString user = users[client];
            qDebug() << "User:" << user;
            qDebug() << "Message:" << message;
            if (line == "help") {
                message += "\n";
                message += "Grundbefehle:\n";
                message += "->print variables\n";
                message += "->print functions\n";
                message += "->print commands") + "\n";
                message += "\n");
                message += "Definitionen:") + "\n";
                message += "->f(x,y):=x^sin(x)") + "\n";
                message += QString("->var:=3.5" + "\n");
                message += QString("\n");
                message += QString("Loeschen:" + "\n");
                message += QString("->delete variable $identifier") + "\n";
                message += QString("->delete function $identifier") + "\n";
                message += QString("->clear variables") + "\n";
                message += QString("->clear functions") + "\n";
                message += QString("\n");
                message += QString("Ausdruecke auswerten:") + "\n";
                message += "->integrate(x^2,x,0,1,100) (Funktion, Variable, untere Grenze, obere Grenze, Teilintervalle)\n";
                message += QString("->Grundrechenarten, sqrt(), log(), ln(), sin(), cos(), tan(), funktion(3,4)") + "\n";
                message += QString("->$n steht fuer den Ausdruck des n-ten Kommandos" + "\n";
                message += QString("\n";
                message += QString("\n";
            } else if (line == "quit") exit(0);
            else if (line == "print variables") {
                vector<Variable> varvec = mycas.getVariables();
                for (vector<Variable>::iterator it = varvec.begin(); it != varvec.end(); ++it)
                    message += "[" + it - varvec.begin() + 1 + "]\t" + *it + "\n";
            } else if (line == "print functions") {
                vector<Function> funcvec = mycas.getFunctions();
                for (vector<Function>::iterator it = funcvec.begin(); it != funcvec.end(); ++it)
                    message += "[" + it - funcvec.begin() + 1 + "]\t" + *it + "\n";
            } else if (line == "print commands") {
                vector<Command> comvec = mycas.getCommands();
                for (vector<Command>::iterator it = comvec.begin(); it != comvec.end(); ++it)
                    message += "[" + it - comvec.begin() + 1 + "]\t" + *it + "\n";
            } else if (line.substr(0, 15) == "delete variable") {
                mycas.deleteVariable(line.substr(16, line.length() - 16));
            } else if (line.substr(0, 15) == "delete function") {
                mycas.deleteFunction(line.substr(16, line.length() - 16));
            } else if (line.substr(0, 15) == "clear functions") {
                mycas.clearFunctions();
            } else if (line.substr(0, 15) == "clear variables") {
                mycas.clearVariables();
            } else if (line.substr(0, 5) == "reset") {
                mycas.reset();
            } else {
                try {
                    message += mycas.process(line) + "\n";
                } catch (const char *s) {
                    message += s + "\n";
                }
            }

            foreach(QTcpSocket *otherClient, clients)
                otherClient->write(message);
        }
        else
        {
            qWarning() << "Got bad message from client:" << client->peerAddress().toString() << line;
        }
    }
}

void ChatterBoxServer::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    clients.remove(client);

    QString user = users[client];
    users.remove(client);

    sendUserList();
    foreach(QTcpSocket *client, clients)
        client->write(QString("Server:" + user + " has left.\n").toUtf8());
}

void ChatterBoxServer::sendUserList()
{
    QStringList userList;
    foreach(QString user, users.values())
        userList << user;

    foreach(QTcpSocket *client, clients)
        client->write(QString("/users:" + userList.join(",") + "\n").toUtf8());
}
