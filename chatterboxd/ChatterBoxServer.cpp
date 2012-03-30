#include "ChatterBoxServer.h"

#include <QTcpSocket>
#include <QRegExp>

ChatterBoxServer::ChatterBoxServer(QObject *parent) : QTcpServer(parent)
{
    //create CAS for global scope:
    namecas["global"] = new CAS;
}

void ChatterBoxServer::incomingConnection(int socketfd)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketfd);
    clients.insert(client);

    //assign user to global scope:
    usercas[client] = namecas["global"];
    username[client] = "global";

    //tell user his scope:
    client->write(QString("scope:global\n").toUtf8());

    //report new user to server console:
    qDebug() << "New client from:" << client->peerAddress().toString();

    //connect to socket signals:
    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void ChatterBoxServer::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    while(client->canReadLine())
    {
        //read line and remove whitespaces:
        QString line = QString::fromUtf8(client->readLine()).trimmed();
        //write to server console:
        qDebug() << "Read line:" << line;

        if(line.left(line.indexOf(':')) == "me") //new user
        {
            //username:
            QString user = line.right(line.length() - line.indexOf(':') - 1);
            //add user to socket/user list:
            users[client] = user;
            //send "new user"-message to all clients:
            foreach(QTcpSocket *client, clients)
                client->write(QString("msg:Server:" + user + " has joined.\n").toUtf8());
            //update user list:
            sendUserList();
        }
        else if(users.contains(client) && line.left(line.indexOf(':')) == "msg")
        {
            QString message = "<b>CAS</b>:";
            QString user = users[client];
            line = line.right(line.length() - line.indexOf(':') - 1);
            qDebug() << "User:" << user;
            qDebug() << "Message:" << line;

            if (line == "help") {
                message += "\n";
                message += "Grundbefehle:\n";
                message += "->print variables\n";
                message += "->print functions\n";
                message += "->print commands\n\n";
                message += "Definitionen:\n";
                message += "->f(x,y):=x^sin(x)\n\n";
                message += "->var:=3.5\n";
                message += "Loeschen:\n";
                message += "->delete variable $identifier\n";
                message += "->delete function $identifier\n";
                message += "->clear variables\n";
                message += "->clear functions\n\n";
                message += "Ausdruecke auswerten:\n";
                message += "->integrate(x^2,x,0,1,100) (Funktion, Variable, untere Grenze, obere Grenze, Teilintervalle)\n";
                message += "->Grundrechenarten, sqrt(), log(), ln(), sin(), cos(), tan(), funktion(3,4)\n";
                message += "->$n steht fuer den Ausdruck des n-ten Kommandos\n\n\n";
            } else if (line == "quit") exit(0);
            else if (line == "print variables") {
                vector<Variable> varvec = usercas[client]->getVariables();
                for (vector<Variable>::iterator it = varvec.begin(); it != varvec.end(); ++it)
                    message += QString("[") + QString::number(it - varvec.begin() + 1) + QString("]\t") + QString::fromStdString(it->getString()) + "\n";
            } else if (line == "print functions") {
                vector<Function> funcvec = usercas[client]->getFunctions();
                for (vector<Function>::iterator it = funcvec.begin(); it != funcvec.end(); ++it)
                    message += QString("[") + QString::number(it - funcvec.begin() + 1) + QString("]\t") + QString::fromStdString(it->getString()) + "\n";
            } else if (line == "print commands") {
                vector<Command> comvec = usercas[client]->getCommands();
                for (vector<Command>::iterator it = comvec.begin(); it != comvec.end(); ++it)
                    message += QString("[") + QString::number(it - comvec.begin() + 1) + QString("]\t") + QString::fromStdString(it->getString()) + "\n";
            } else if (line.left(15) == "delete variable") {
                usercas[client]->deleteVariable(line.right(line.length() - 16).toStdString());
            } else if (line.left(15) == "delete function") {
                usercas[client]->deleteFunction(line.right(line.length() - 16).toStdString());
            } else if (line.left(15) == "clear functions") {
                usercas[client]->clearFunctions();
            } else if (line.left(15) == "clear variables") {
                usercas[client]->clearVariables();
            } else if (line.left(5) == "reset") {
                usercas[client]->reset();
            } else if (line == "print scopes") {
                for (QMap<QString, CAS*>::iterator it = namecas.begin(); it != namecas.end(); ++it)
                    message += it.key() + "\n";
            } else {
                try {
                    message += QString::fromStdString(usercas[client]->process(line.toStdString()));
                } catch (const char* s) {
                    message += s;
                }
            }

            foreach(QTcpSocket *otherClient, clients)
                otherClient->write(QString("<b>" + user + "</b>:" + line + "\n" + message + "\n").toUtf8());
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
    //write to server console:
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    //remove from client and user lists:
    clients.remove(client);

    QString user = users[client];
    users.remove(client);

    //update user lists:
    sendUserList();
    //send message to clients:
    foreach(QTcpSocket *client, clients)
        client->write(QString("msg:Server:" + user + " has left.\n").toUtf8());
}

void ChatterBoxServer::sendUserList()
{
    QStringList userList;
    //fill StringList with "username (scope)"
    for (QMap<QTcpSocket*,QString>::iterator it = users.begin(); it != users.end(); ++it)
        userList << QString(it.value() + " (" + username[it.key()] + ")");

    //send to clients:
    foreach(QTcpSocket *client, clients)
        client->write(QString("ul:" + userList.join(",") + "\n").toUtf8());
}
