#ifndef __ChatterBoxServer_H__
#define __ChatterBoxServer_H__

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>
#include <QList>
#include <QVector>
#include "CAS.h"

class ChatterBoxServer : public QTcpServer
{
    Q_OBJECT

    public:
        ChatterBoxServer(QObject *parent=0);

    private slots:
        void readyRead();
        void disconnected();

    protected:
        void incomingConnection(int socketfd);

    private:
        QSet<QTcpSocket*> clients;
        QMap<QTcpSocket*,QString> users;
        QMap<QString, QList<QTcpSocket*> > socketsbyscope;
        QMap<QTcpSocket*, QString> scopebysocket;
        QMap<QString, CAS*> casbyscope;

        void sendUserListToScope(QList<QTcpSocket*>);
        void sendScopeList(QTcpSocket*);
        void sendFunctions(QTcpSocket*);
        void sendVariables(QTcpSocket*);
        void sendCommands(QTcpSocket*);
        void addUserToScope(QTcpSocket*, QString);
        void deleteUser(QTcpSocket*);
        void deleteUserFromScope(QTcpSocket*, QString);
        void changeUserScope(QTcpSocket*, QString);
        void deleteScope(QString);
};

#endif
