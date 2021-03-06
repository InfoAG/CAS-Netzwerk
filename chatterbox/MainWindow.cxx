#include "MainWindow.h"

// This is our MainWindow constructor (you C++ n00b)
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // When using Designer, you should always call setupUi(this)
    // in your constructor. This creates and lays out all the widgets
    // on the MainWindow that you setup in Designer.
    setupUi(this);

    connect(userLineEdit, SIGNAL(textChanged(QString)), this, SLOT(userTextChanged(QString)));
    connect(portLineEdit, SIGNAL(textChanged(QString)), this, SLOT(portTextChanged(QString)));
    invalidLabel->setStyleSheet("QLabel { color : red; }");
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(invalidLabel);
    invalidLabel->setGraphicsEffect(effect);
    anim = new QPropertyAnimation(effect, "opacity");
    anim->setStartValue(1.0);
    anim->setKeyValueAt(0.5, 1.0);
    anim->setEndValue(0);
    anim->setDuration(2000);

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

    connect(scopeListWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(customScopeContextMenuRequested(const QPoint&)));
    connect(sayTextEdit, SIGNAL(cHistRequested(QKeyEvent*)), this, SLOT(cHistRequested(QKeyEvent*)));
    connect(serverLineEdit, SIGNAL(returnPressed()), this, SLOT(serverReturnPressed()));
    connect(portLineEdit, SIGNAL(returnPressed()), this, SLOT(portReturnPressed()));
    connect(portLineEdit, SIGNAL(textChanged(QString)), this, SLOT(anyTextChanged(QString)));
    connect(serverLineEdit, SIGNAL(textChanged(QString)), this, SLOT(anyTextChanged(QString)));
    connect(userLineEdit, SIGNAL(textChanged(QString)), this, SLOT(anyTextChanged(QString)));
    connect(scopeListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(currentItemChanged(QListWidgetItem*, QListWidgetItem*)));
    connect(scopeListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(itemChanged(QListWidgetItem*)));
    connect(functionListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(scopeInfo_itemClicked(QListWidgetItem*)));
    connect(variableListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(scopeInfo_itemClicked(QListWidgetItem*)));
    connect(commandListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(scopeInfo_itemClicked(QListWidgetItem*)));

#ifdef AUTOCONNECT
    serverLineEdit->setText("localhost");
    qsrand((uint)QTime::currentTime().msec());
    userLineEdit->setText("user" + QString::number(qrand()));
    loginButton->click();
#endif //AUTOCONNECT
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
    if (userLineEdit->text() == "CAS") {
        QMessageBox::information(this, "CAS Client", "I'm pretty sure you're overestimating your math skills.\n"
                                                     "If you are a computer or alien and interested in a job, contact \"InfoAG\" on github.");
    } else {
        socket->connectToHost(serverLineEdit->text(), portLineEdit->text().toInt());
        loginButton->setEnabled(false);
        loginButton->setText("connecting");
    }
}

// This gets called when the user clicks the sayButton (next to where
// they type text to send to the chat room):
void MainWindow::on_sayButton_clicked()
{
    // What did they want to say (minus white space around the string):
    QString message = this->sayTextEdit->toPlainText().trimmed();

    // Only send the text to the chat server if it's not empty:
    if(!message.isEmpty())
    {
        socket->write(QString("msg:" + message + "\n").toUtf8());
        if (commandHistory.isEmpty() || message != commandHistory.last()) {
            commandHistory << message;
            cHistPos = commandHistory.length();
        }
    }

    // Clear out the input box so they can type something else:
    this->sayTextEdit->clear();

    // Put the focus back into the input box so they can type again:
    this->sayTextEdit->setFocus();
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

        if (line == "chokai") {
            // Flip over to the chat page:
            stackedWidget->setCurrentWidget(chatPage);
        } else if (line == "inuse") {
            socket->disconnectFromHost();
#ifdef AUTOCONNECT
            userLineEdit->setText("user" + QString::number(qrand()));
            loginButton->click();
#else
            QMessageBox::information(this, "CAS Client", "The username \"" + userLineEdit->text() + "\" is already in use. Please choose a different one.");
#endif //AUTOCONNECT
        // Is this a user list message:
        }else if(line.left(line.indexOf(':')) == "ul")
        {
            // If so, udpate our users list on the right:
            QStringList users = line.right(line.length() - 3).split(";");
            userListWidget->clear();
            QListWidgetItem *lwi = new QListWidgetItem(QPixmap(":/cas.png"), "CAS", userListWidget);
            lwi->setToolTip(socket->peerAddress().toString());

            int pos;
            foreach(QString user, users) {
                pos = user.indexOf(":");
                lwi = new QListWidgetItem(QPixmap(":/user.png"), user.left(pos), userListWidget);
                lwi->setToolTip(user.right(user.length() - pos - 1));
            }

        } else if(line.left(line.indexOf(':')) == "sl") //scope list update
        {
            // If so, udpate our scope list on the left:
            QStringList scopes = line.right(line.length() - 3).split(";");
            scopeListWidget->clear();
            QMap<QString, QListWidgetItem*> listitembyscope;
            foreach(QString scope, scopes) {
                QListWidgetItem *lwi = new QListWidgetItem(scope);
                listitembyscope[scope] = lwi;
                scopeListWidget->addItem(lwi);
                if (! texteditbyscope.contains(scope)) {
                    QTextEdit *te = new roomTextEdit;
                    te->setReadOnly(true);
                    te->installEventFilter(this);
                    texteditbyscope[scope] = te;
                    stackedRooms->addWidget(te);
                }
            }
            newScope = new QListWidgetItem("New Scope");
            newScope->setFlags(newScope->flags() | Qt::ItemIsEditable);
            scopeListWidget->sortItems();
            scopeListWidget->addItem(newScope);
            if (! listitembyscope.contains(currentScope)) currentScope = "global";
            scopeListWidget->setCurrentItem(listitembyscope[currentScope]);
            stackedRooms->setCurrentWidget(texteditbyscope[currentScope]);

        } else if(line.left(line.indexOf(':')) == "fl")
        {
                // If so, udpate our functions list on the right:
                QStringList items = line.right(line.length() - 3).split(";");
                functionListWidget->clear();

                foreach(QString item, items) {
                    functionListWidget->addItem(item);
                }
        } else if(line.left(line.indexOf(':')) == "vl")
        {
                // If so, udpate our variables list on the right:
                QStringList items = line.right(line.length() - 3).split(";");
                variableListWidget->clear();

                foreach(QString item, items) {
                    variableListWidget->addItem(item);
                }
        } else if(line.left(line.indexOf(':')) == "cl")
        {
                // If so, udpate our commands list on the right:
                QStringList items = line.right(line.length() - 3).split(";");
                commandListWidget->clear();

                foreach(QString item, items) {
                    commandListWidget->addItem(item);
                }

        // Is this a normal chat message:
        } else if(line.left(line.indexOf(':')) == "msg")
        {
            // If so, append this message to our chat box:
            int posfirst = line.indexOf(':'), possecond = line.indexOf(':', posfirst + 1), posthird = line.indexOf(':', possecond + 1);
            QString scope = line.mid(posfirst + 1, possecond - 4);
            QString user = line.mid(possecond + 1, posthird - possecond - 1);
            QString message = line.right(line.length() - posthird - 1);

            if (scope.isEmpty()) {
                foreach (QTextEdit *te, texteditbyscope.values())
                    te->append("<b>" + user + "</b>: " + message);
            } else texteditbyscope[scope]->append("<b>" + user + "</b>: " + message);
        }
    }
}

// This function gets called when our socket has successfully connected to the chat
// server. (see the connect() call in the MainWindow constructor).
void MainWindow::connected()
{
    // And send our username to the chat server.
    socket->write(QString("me:" + userLineEdit->text() + "\n").toUtf8());
    currentScope = "global";
    cHistPos = -1;
}

void MainWindow::disconnected()
{
    texteditbyscope.clear();

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

void MainWindow::currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous) {
    if ((! (previous == NULL && current->text() == "global")) && current != NULL && current != newScope) {
        userListWidget->clear();
        socket->write(QString("scope:" + current->text() + "\n").toUtf8());
        stackedRooms->setCurrentWidget(texteditbyscope[current->text()]);
        currentScope = current->text();
    }
}

void MainWindow::itemChanged(QListWidgetItem* item) {
    if (item == newScope) {
        if (item->text().isEmpty()) item->setText("New Scope");
        else if (item->text().contains(",") || item->text().contains(":")) {
            item->setText("New Scope");
            QMessageBox::information(this, "New Scope", "Scope names cannot contain \",\" or \":\". Please choose a different name.");
        }
        else if (item->text() != "New Scope") {
            socket->write(QString("scope:" + item->text() + "\n").toUtf8());
            currentScope = item->text();
        }
    }
}

void MainWindow::customScopeContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *under = scopeListWidget->itemAt(pos);
    if(under && under != newScope && under->text() != "global") {
        QMenu *menu = new QMenu(this);
        menu->addAction("Delete Scope", this, SLOT(deleteScope()));
        menu->exec(scopeListWidget->mapToGlobal(pos));
    }
}

void MainWindow::cHistRequested(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Up && cHistPos > 0) {
        sayTextEdit->setText(commandHistory.at(--cHistPos));
    } else if (e->key() == Qt::Key_Down) {
        if (cHistPos == commandHistory.length() - 1) {
            sayTextEdit->setText("");
            ++cHistPos;
        } else if (cHistPos < commandHistory.length() - 1) sayTextEdit->setText(commandHistory.at(++cHistPos));
    }
    QTextCursor cursor = sayTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    sayTextEdit->setTextCursor(cursor);
}

void MainWindow::scopeInfo_itemClicked(QListWidgetItem *item)
{
    sayTextEdit->setText(sayTextEdit->toPlainText() + item->text());
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == stackedRooms->currentWidget()) {
        if (event->type() == QEvent::ToolTip) {
            QHelpEvent *helpEvent = static_cast<QHelpEvent*>(event);
            QTextCursor cursor = static_cast<QTextEdit*>(obj)->cursorForPosition(helpEvent->pos());
            cursor.select(QTextCursor::WordUnderCursor); //problem: needs whitespaces around expression; no syntax checking
            QString selected = cursor.selectedText();
            cursor.movePosition(QTextCursor::EndOfWord);
            cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

            if (cursor.selectedText() == "(") {
                foreach (QListWidgetItem *widget, functionListWidget->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard)) {
                    if (widget->text().left(widget->text().indexOf("(")) == selected) {
                        QToolTip::showText(helpEvent->globalPos(), widget->text().right(widget->text().length() - widget->text().indexOf("=") - 1));
                        return true;
                    }
                }
            } else {
                cursor.movePosition(QTextCursor::StartOfWord);
                cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
                if (cursor.selectedText() == "$") {
                    int whichCommand = selected.toInt();
                    if (whichCommand > 0 && whichCommand <= commandListWidget->count()) {
                        QToolTip::showText(helpEvent->globalPos(), commandListWidget->item(whichCommand - 1)->text());
                        return true;
                    }
                } else {
                    int pos;
                    foreach (QListWidgetItem *widget,  variableListWidget->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard)) {
                        pos = widget->text().indexOf("=");
                        if (widget->text().left(pos) == selected) {
                            QToolTip::showText(helpEvent->globalPos(), widget->text().right(widget->text().length() - pos - 1));
                            return true;
                        }
                    }
                }
            }

            //TODO: commands $n -> nth command's value

            QToolTip::hideText();
            return true;
        } else return false;
    } else return QMainWindow::eventFilter(obj, event);
}

void MainWindow::deleteScope()
{
    socket->write(QString("ds:" + scopeListWidget->currentItem()->text() + "\n").toUtf8());
}

void MainWindow::userTextChanged(const QString& text)
{
    if (text.contains(",") || text.contains(":")) {
        invalidLabel->setText("Usernames cannot contain \",\" or \":\".");
        userLineEdit->setText(userLineEdit->text().replace(",", ""));
        userLineEdit->setText(userLineEdit->text().replace(":", ""));
        QApplication::beep();
        anim->stop();
        anim->start(QAbstractAnimation::KeepWhenStopped);
    }
}

void MainWindow::portTextChanged(const QString &text)
{
    bool isNumber = false;
    text.toInt(&isNumber);
    if (! isNumber) {
        invalidLabel->setText("Ports are numbers. Idiot.");
        portLineEdit->setText(portLineEdit->text().remove(QRegExp("[^0-9]*")));
        QApplication::beep();
        anim->stop();
        anim->start(QAbstractAnimation::KeepWhenStopped);
    }
}

void MainWindow::anyTextChanged(const QString &)
{
    if (userLineEdit->text().isEmpty() || portLineEdit->text().isEmpty() || serverLineEdit->text().isEmpty())
        loginButton->setEnabled(false);
    else loginButton->setEnabled(true);
}

void MainWindow::serverReturnPressed()
{
    if (portLineEdit->text().isEmpty()) portLineEdit->setFocus();
    else if (userLineEdit->text().isEmpty()) userLineEdit->setFocus();
    else loginButton->animateClick();
}

void MainWindow::portReturnPressed()
{
    if (userLineEdit->text().isEmpty()) userLineEdit->setFocus();
    else loginButton->animateClick();
}

