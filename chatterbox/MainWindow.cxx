#include "MainWindow.h"

// We'll need some regular expression magic in this code:
#include <QRegExp>
#include <QtGui>

// This is our MainWindow constructor (you C++ n00b)
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // When using Designer, you should always call setupUi(this)
    // in your constructor. This creates and lays out all the widgets
    // on the MainWindow that you setup in Designer.
    setupUi(this);

    // Make sure that we are showing the login page when we startup:
    stackedWidget->setCurrentWidget(loginPage);

    // Instantiate our socket (but don't actually connect to anything
    // yet until the user clicks the loginButton:
    socket = new QTcpSocket(this);

    // This is how we tell Qt to call our readyRead() and connected()
    // functions when the socket has text ready to be read, and is done
    // connecting to the server (respectively):
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));

    //handle disconnect and error signals:
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}

// This gets called when the loginButton gets clicked:
// We didn't have to use connect() to set this up because
// Qt recognizes the name of this function and knows to set
// up the signal/slot connection for us.
void MainWindow::on_loginButton_clicked()
{
    // Start connecting to the chat server (on port 4200).
    // This returns immediately and then works on connecting
    // to the server in the background. When it's done, we'll
    // get a connected() function call (below). If it fails,
    // we won't get any error message because we didn't connect()
    // to the error() signal from this socket.
    socket->connectToHost(serverLineEdit->text(), 4200);
    loginButton->setEnabled(false);
    loginButton->setText("connecting");
}

// This gets called when the user clicks the sayButton (next to where
// they type text to send to the chat room):
void MainWindow::on_sayButton_clicked()
{
    // What did they want to say (minus white space around the string):
    QString message = sayLineEdit->text().trimmed();

    // Only send the text to the chat server if it's not empty:
    if(!message.isEmpty())
    {
        socket->write(QString("msg:" + message + "\n").toUtf8());
    }

    // Clear out the input box so they can type something else:
    sayLineEdit->clear();

    // Put the focus back into the input box so they can type again:
    sayLineEdit->setFocus();
}

// This function gets called whenever the chat server has sent us some text:
void MainWindow::readyRead()
{
    // We'll loop over every (complete) line of text that the server has sent us:
    while(socket->canReadLine())
    {
        // Here's the line the of text the server sent us (we use UTF-8 so
        // that non-English speakers can chat in their native language)
        QString line = QString::fromUtf8(socket->readLine()).trimmed();

        // Is this a user list message:
        if(line.left(line.indexOf(':')) == "ul")
        {
            // If so, udpate our users list on the right:
            QStringList users = line.right(line.length() - line.indexOf(':') - 1).split(",");
            userListWidget->clear();
            new QListWidgetItem(QPixmap(":/cas.png"), "CAS", userListWidget);
            foreach(QString user, users)
                new QListWidgetItem(QPixmap(":/user.png"), user, userListWidget);
        } else if(line.left(line.indexOf(':')) == "scope") titleLabel->setText("CAS Client (" + line.right(line.length() - line.indexOf(':') - 1) + ")");
        // Is this a normal chat message:
        else if(line.left(line.indexOf(':')) == "msg")
        {
            // If so, append this message to our chat box:
            int posfirst = line.indexOf(':'), possecond = line.indexOf(':', posfirst + 1);
            QString user = line.mid(posfirst + 1, possecond - 4);
            QString message = line.right(line.length() - possecond - 1);

            roomTextEdit->append("<b>" + user + "</b>: " + message);
        } else roomTextEdit->append(line);
    }
}

// This function gets called when our socket has successfully connected to the chat
// server. (see the connect() call in the MainWindow constructor).
void MainWindow::connected()
{
    // Flip over to the chat page:
    stackedWidget->setCurrentWidget(chatPage);

    // And send our username to the chat server.
    socket->write(QString("me:" + userLineEdit->text() + "\n").toUtf8());
}

void MainWindow::disconnected()
{
    //prepare loginPage:
    loginButton->setText("Login");
    loginButton->setEnabled(true);

    //switch to loginPage:
    stackedWidget->setCurrentWidget(loginPage);
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    //reset login-Button:
    loginButton->setText("Login");
    loginButton->setEnabled(true);

    //show error message:
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("CAS Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("CAS Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the CAS server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("CAS Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(socket->errorString()));
    }
}
