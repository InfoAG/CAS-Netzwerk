/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Wed 18. Apr 10:08:48 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStackedWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <onelinetextedit.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QFrame *mainFrame;
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *stackedWidget;
    QWidget *chatPage;
    QGridLayout *gridLayout;
    QListWidget *userListWidget;
    QPushButton *sayButton;
    QListWidget *scopeListWidget;
    QStackedWidget *stackedRooms;
    OneLineTextEdit *sayTextEdit;
    QWidget *loginPage;
    QGridLayout *gridLayout_3;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QFrame *loginFrame;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QLineEdit *serverLineEdit;
    QLabel *label_2;
    QLineEdit *userLineEdit;
    QPushButton *loginButton;
    QLabel *invalidLabel;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        MainWindow->setContextMenuPolicy(Qt::DefaultContextMenu);
        MainWindow->setStyleSheet(QString::fromUtf8("#titleLabel {\n"
"background: white;\n"
"color: blue;\n"
"font-size: 20px;\n"
"border: none;\n"
"border-bottom:  1px solid black;\n"
"padding: 5px;\n"
"}\n"
"\n"
"#mainFrame {\n"
"border: none;\n"
"background: white;\n"
"}\n"
"\n"
"#loginFrame {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ddf, stop: 1 #aaf);\n"
"border: 1px solid gray;\n"
"padding: 10px;\n"
"border-radius: 25px;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(titleLabel->sizePolicy().hasHeightForWidth());
        titleLabel->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(titleLabel);

        mainFrame = new QFrame(centralwidget);
        mainFrame->setObjectName(QString::fromUtf8("mainFrame"));
        mainFrame->setFrameShape(QFrame::StyledPanel);
        verticalLayout_2 = new QVBoxLayout(mainFrame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        stackedWidget = new QStackedWidget(mainFrame);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        chatPage = new QWidget();
        chatPage->setObjectName(QString::fromUtf8("chatPage"));
        gridLayout = new QGridLayout(chatPage);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        userListWidget = new QListWidget(chatPage);
        userListWidget->setObjectName(QString::fromUtf8("userListWidget"));

        gridLayout->addWidget(userListWidget, 0, 2, 1, 2);

        sayButton = new QPushButton(chatPage);
        sayButton->setObjectName(QString::fromUtf8("sayButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sayButton->sizePolicy().hasHeightForWidth());
        sayButton->setSizePolicy(sizePolicy1);
        sayButton->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(sayButton, 1, 3, 1, 1);

        scopeListWidget = new QListWidget(chatPage);
        scopeListWidget->setObjectName(QString::fromUtf8("scopeListWidget"));
        scopeListWidget->setMaximumSize(QSize(100, 16777215));
        scopeListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        scopeListWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
        scopeListWidget->setSortingEnabled(false);

        gridLayout->addWidget(scopeListWidget, 0, 0, 1, 1);

        stackedRooms = new QStackedWidget(chatPage);
        stackedRooms->setObjectName(QString::fromUtf8("stackedRooms"));

        gridLayout->addWidget(stackedRooms, 0, 1, 1, 1);

        sayTextEdit = new OneLineTextEdit(chatPage);
        sayTextEdit->setObjectName(QString::fromUtf8("sayTextEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sayTextEdit->sizePolicy().hasHeightForWidth());
        sayTextEdit->setSizePolicy(sizePolicy2);
        sayTextEdit->setMaximumSize(QSize(682, 22));
        sayTextEdit->setFocusPolicy(Qt::StrongFocus);
        sayTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sayTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sayTextEdit->setTabChangesFocus(true);
        sayTextEdit->setLineWrapMode(QTextEdit::NoWrap);

        gridLayout->addWidget(sayTextEdit, 1, 0, 1, 3);

        stackedWidget->addWidget(chatPage);
        loginPage = new QWidget();
        loginPage->setObjectName(QString::fromUtf8("loginPage"));
        gridLayout_3 = new QGridLayout(loginPage);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        verticalSpacer = new QSpacerItem(20, 100, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(223, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 1, 0, 1, 1);

        loginFrame = new QFrame(loginPage);
        loginFrame->setObjectName(QString::fromUtf8("loginFrame"));
        sizePolicy1.setHeightForWidth(loginFrame->sizePolicy().hasHeightForWidth());
        loginFrame->setSizePolicy(sizePolicy1);
        loginFrame->setMinimumSize(QSize(300, 0));
        loginFrame->setFrameShape(QFrame::StyledPanel);
        gridLayout_2 = new QGridLayout(loginFrame);
        gridLayout_2->setSpacing(20);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(loginFrame);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        serverLineEdit = new QLineEdit(loginFrame);
        serverLineEdit->setObjectName(QString::fromUtf8("serverLineEdit"));

        gridLayout_2->addWidget(serverLineEdit, 0, 1, 1, 1);

        label_2 = new QLabel(loginFrame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        userLineEdit = new QLineEdit(loginFrame);
        userLineEdit->setObjectName(QString::fromUtf8("userLineEdit"));

        gridLayout_2->addWidget(userLineEdit, 1, 1, 1, 1);

        loginButton = new QPushButton(loginFrame);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        sizePolicy1.setHeightForWidth(loginButton->sizePolicy().hasHeightForWidth());
        loginButton->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(loginButton, 3, 1, 1, 1);

        invalidLabel = new QLabel(loginFrame);
        invalidLabel->setObjectName(QString::fromUtf8("invalidLabel"));

        gridLayout_2->addWidget(invalidLabel, 2, 0, 1, 2);


        gridLayout_3->addWidget(loginFrame, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(223, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 267, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 2, 1, 1, 1);

        stackedWidget->addWidget(loginPage);

        verticalLayout_2->addWidget(stackedWidget);


        verticalLayout->addWidget(mainFrame);

        MainWindow->setCentralWidget(centralwidget);
        QWidget::setTabOrder(serverLineEdit, userLineEdit);
        QWidget::setTabOrder(userLineEdit, loginButton);
        QWidget::setTabOrder(loginButton, sayTextEdit);
        QWidget::setTabOrder(sayTextEdit, sayButton);
        QWidget::setTabOrder(sayButton, scopeListWidget);
        QWidget::setTabOrder(scopeListWidget, userListWidget);

        retranslateUi(MainWindow);
        QObject::connect(serverLineEdit, SIGNAL(returnPressed()), userLineEdit, SLOT(setFocus()));
        QObject::connect(userLineEdit, SIGNAL(returnPressed()), loginButton, SLOT(animateClick()));

        stackedWidget->setCurrentIndex(0);
        stackedRooms->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CAS Client", 0, QApplication::UnicodeUTF8));
        titleLabel->setText(QApplication::translate("MainWindow", "CAS Client", 0, QApplication::UnicodeUTF8));
        sayButton->setText(QApplication::translate("MainWindow", "Process", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Server name:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "User name:", 0, QApplication::UnicodeUTF8));
        loginButton->setText(QApplication::translate("MainWindow", "Login", 0, QApplication::UnicodeUTF8));
        invalidLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
