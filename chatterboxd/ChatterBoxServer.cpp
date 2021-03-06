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
            if (! usernames.contains(user)) {
                //add user to socket/user list:
                users[client] = user;
                usernames.push_back(user);
                //send "new user"-message to all clients:
                foreach(QTcpSocket *client, clients)
                    client->write(QString("msg::Server:" + user + " has joined.\n").toUtf8());
                addUserToScope(client, "global");
                sendScopeList(client);
                client->write(QString("chokai\n").toUtf8());
            } else client->write(QString("inuse\n").toUtf8());
        }
        else if(users.contains(client))
        {
            if (line.left(line.indexOf(':')) == "msg") {
                QStringList message;
                QString user = users[client];
                line = line.right(line.length() - line.indexOf(':') - 1);
                qDebug() << "User:" << user;
                qDebug() << "Message:" << line;

                if (line == "help") {
                    message << "\n";
                    message << "Grundbefehle:\n";
                    message << "->print variables\n";
                    message << "->print functions\n";
                    message << "->print commands\n\n";
                    message << "Definitionen:\n";
                    message << "->f(x,y):=x^sin(x)\n\n";
                    message << "->var:=3.5\n";
                    message << "Loeschen:\n";
                    message << "->delete variable $identifier\n";
                    message << "->delete function $identifier\n";
                    message << "->clear variables\n";
                    message << "->clear functions\n\n";
                    message << "Ausdruecke auswerten:\n";
                    message << "->integrate(x^2,x,0,1,100) (Funktion, Variable, untere Grenze, obere Grenze, Teilintervalle)\n";
                    message << "->Grundrechenarten, sqrt(), log(), ln(), sin(), cos(), tan(), funktion(3,4)\n";
                    message << "->$n steht fuer den Ausdruck des n-ten Kommandos\n\n\n";
                } else if (line == "quit") exit(0);
                else if (line == "print variables") {
                    vector<Variable> varvec = casbyscope[scopebysocket[client]]->getVariables();
                    if (varvec.empty()) message << "No variables to show.";
                    else {
                        for (vector<Variable>::iterator it = varvec.begin(); it != varvec.end(); ++it)
                            message << QString("[") + QString::number(it - varvec.begin() + 1) + QString("]\t") + QString::fromStdString(it->getString()) + "\n";
                    }
                } else if (line == "print functions") {
                    vector<Function> funcvec = casbyscope[scopebysocket[client]]->getFunctions();
                    if (funcvec.empty()) message << "No functions to show.";
                    else {
                    for (vector<Function>::iterator it = funcvec.begin(); it != funcvec.end(); ++it)
                        message << QString("[") + QString::number(it - funcvec.begin() + 1) + QString("]\t") + QString::fromStdString(it->getString()) + "\n";
                    }
                } else if (line == "print commands") {
                    vector<Command> comvec = casbyscope[scopebysocket[client]]->getCommands();
                    if (comvec.empty()) message << "No commands to show.";
                    else {
                        for (vector<Command>::iterator it = comvec.begin(); it != comvec.end(); ++it)
                            message << QString("[") + QString::number(it - comvec.begin() + 1) + QString("]\t") + QString::fromStdString(it->getString()) + "\n";
                    }
                } else if (line.left(15) == "delete variable") {
                    casbyscope[scopebysocket[client]]->deleteVariable(line.right(line.length() - 16).toStdString());
                    message << "Variable deleted.";
                } else if (line.left(15) == "delete function") {
                    casbyscope[scopebysocket[client]]->deleteFunction(line.right(line.length() - 16).toStdString());
                    message << "Function deleted.";
                } else if (line.left(15) == "clear functions") {
                    casbyscope[scopebysocket[client]]->clearFunctions();
                    message << "Functions deleted.";
                } else if (line.left(15) == "clear variables") {
                    casbyscope[scopebysocket[client]]->clearVariables();
                    message << "Variables deleted.";
                } else if (line.left(5) == "reset") {
                    casbyscope[scopebysocket[client]]->reset();
                    message << "Reset successful.";
                } else {
                    try {
                        message << QString::fromStdString(casbyscope[scopebysocket[client]]->process(line.toStdString()));
                    } catch (const char* s) {
                        message << s;
                    }
                }
                foreach(QTcpSocket *otherClient, clients)
                    otherClient->write(QString("msg:" + scopebysocket[client] + ":" + user + ":" + line + "\nmsg:" + scopebysocket[client] + ":CAS:" + message.join("\nmsg:" + scopebysocket[client] + ":CAS:") + "\n").toUtf8());
                QString clientScope = scopebysocket[client];
                CAS *clientCAS = casbyscope[clientScope];
                QList<QTcpSocket*> scopeSockets = socketsbyscope[clientScope];
                if (clientCAS->functionsModified()) {
                    foreach (QTcpSocket *socket, scopeSockets)
                        sendFunctions(socket);
                }
                if (clientCAS->variablesModified()) {
                    foreach (QTcpSocket *socket, scopeSockets)
                        sendVariables(socket);
                }
                if (clientCAS->commandsModified()) {
                    foreach (QTcpSocket *socket, scopeSockets)
                        sendCommands(socket);
                }

            } else if (line.left(line.indexOf(':')) == "scope") {
                QString scopename = line.right(line.length() - line.indexOf(':') - 1);
                changeUserScope(client, scopename);
            } else if (line.left(line.indexOf(':')) == "ds") {
                QString scopename = line.right(line.length() - line.indexOf(':') - 1);
                deleteScope(scopename);
            }
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
    usernames.removeOne(user);

    deleteUser(client);
    //send message to clients:
    foreach(QTcpSocket *client, clients)
        client->write(QString("msg::Server:" + user + " has left.\n").toUtf8());
}

void ChatterBoxServer::sendUserListToScope(QList<QTcpSocket*> scopeclients)
{
    QStringList userList;
    foreach(QTcpSocket *client, scopeclients)
        userList << users[client] + ":" + client->peerAddress().toString();

    //send to clients:
    foreach(QTcpSocket *client, scopeclients)
        client->write(QString("ul:" + userList.join(";") + "\n").toUtf8());
}

void ChatterBoxServer::sendScopeList(QTcpSocket *socket = NULL) {
    QStringList scopeList;
    foreach(QString scope, socketsbyscope.keys())
        scopeList << scope;

    if (socket) socket->write(QString("sl:" + scopeList.join(";") + "\n").toUtf8());
    else {
        foreach(QTcpSocket *client, clients)
            client->write(QString("sl:" + scopeList.join(";") + "\n").toUtf8());
    }
}

void ChatterBoxServer::sendFunctions(QTcpSocket *socket) {
    QStringList functionList;
    foreach (Function function, casbyscope[scopebysocket[socket]]->getFunctions())
        functionList << QString::fromStdString(function.getString());
    socket->write(QString("fl:" + functionList.join(";") + "\n").toUtf8());
}

void ChatterBoxServer::sendVariables(QTcpSocket *socket) {
    QStringList variableList;
    foreach (Variable variable, casbyscope[scopebysocket[socket]]->getVariables())
        variableList << QString::fromStdString(variable.getString());
    socket->write(QString("vl:" + variableList.join(";")).toUtf8() + "\n");
}

void ChatterBoxServer::sendCommands(QTcpSocket *socket) {
    QStringList commandList;
    foreach (Command command, casbyscope[scopebysocket[socket]]->getCommands())
        commandList << QString::fromStdString(command.getString());
    socket->write(QString("cl:" + commandList.join(";")).toUtf8() + "\n");
}

void ChatterBoxServer::addUserToScope(QTcpSocket* socket, QString scope) {
    if (! socketsbyscope.contains(scope)) {
        casbyscope[scope] = new CAS;
        socketsbyscope[scope].push_back(socket);
        sendScopeList();
    } else socketsbyscope[scope].push_back(socket);
    scopebysocket[socket] = scope;
    sendUserListToScope(socketsbyscope[scope]);
    sendFunctions(socket);
    sendVariables(socket);
    sendCommands(socket);
}

void ChatterBoxServer::deleteUser(QTcpSocket* socket) {
    scopebysocket.remove(socket);

    int pos;
    for (QMap<QString, QList<QTcpSocket*> >::iterator it = socketsbyscope.begin(); it != socketsbyscope.end(); ++it) {
        pos = it.value().indexOf(socket);
        if (pos != -1) {
            it.value().removeOne(socket);
            sendUserListToScope(it.value());
        }
    }
}

void ChatterBoxServer::changeUserScope(QTcpSocket* socket, QString scope) {
    deleteUser(socket);
    addUserToScope(socket, scope);
}

void ChatterBoxServer::deleteScope(QString scope) {
    foreach (QTcpSocket *client, socketsbyscope[scope]) {
        changeUserScope(client, "global");
        socketsbyscope.remove(scope);
        sendScopeList();
    }
}
