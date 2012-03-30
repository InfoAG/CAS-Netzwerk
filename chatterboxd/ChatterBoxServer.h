#ifndef __ChatterBoxServer_H__
#define __ChatterBoxServer_H__

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>
#include "CAS.h"

class ChatterBoxServer : public QTcpServer
{
    Q_OBJECT

    public:
        ChatterBoxServer(QObject *parent=0);

    private slots:
        void readyRead();
        void disconnected();
        void sendUserList();

    protected:
        void incomingConnection(int socketfd);

    private:
        QSet<QTcpSocket*> clients;
        QMap<QTcpSocket*,QString> users;
        CAS globalcas; //CAS for global scope
        QMap<QTcpSocket*, CAS*> usercas; //to look up CAS by socket
        QMap<QString, CAS*> namecas; //to look up CAS by scope name
        QMap<QTcpSocket*, QString> username; //to look up scope name by socket
};

#endif
