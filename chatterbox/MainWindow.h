// We need to include a couple Qt classes that we'll use:
#include <QMainWindow>
#include <QTcpSocket>

// This is the include file that Qt generates for us from the
// GUI we built in Designer  
#include "ui_MainWindow.h"

class User {
private:
    QString name;
public:
    User(QString n) : name(n) {};
    QString getName() const { return name; };
};

QDataStream& operator<<(QDataStream&, const User&);

/*
 * This is the MainWindow class that we have told to inherit from
 * our Designer MainWindow (ui::MainWindow)
 */
class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

    public:

        // Every QWidget needs a constructor, and they should allow
        // the user to pass a parent QWidget (or not).
        MainWindow(QWidget *parent=0);

    private slots:

        // This function gets called when a user clicks on the
        // loginButton on the front page (which you placed there
        // with Designer)
        void on_loginButton_clicked();

        // This gets called when you click the sayButton on
        // the chat page.
        void on_sayButton_clicked();

        // This is a function we'll connect to a socket's readyRead()
        // signal, which tells us there's text to be read from the chat
        // server.
        void readyRead();

        // This function gets called when the socket tells us it's connected.
        void connected();
        void disconnected();
        void displayError(QAbstractSocket::SocketError);

        void currentItemChanged(QListWidgetItem*, QListWidgetItem*);

    private:

        // This is the socket that will let us communitate with the server.
        QTcpSocket *socket;
        QDataStream stream;
};
